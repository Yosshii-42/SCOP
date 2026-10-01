#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "utils/utils.hpp"
#include "parser/tokenizer.hpp"
#include "gl/Shader.hpp"
#include "gl/Window.hpp"
#include "gl/Object.hpp"
#include "gl/Texture.hpp"
#include "math/Mat4.hpp"
#include "math/Vec3.hpp"
#include "math/Vec4.hpp"

int main(int argc, char **argv)
{
  Utils::checkArgv(argc, argv);
  Tokenizer tokenizer(argv[1]);
  
  {
    Window window;                              // window作成
    glEnable(GL_DEPTH_TEST);                    // 深度テストを有効にする
    Shader  ourShader("shaders/vertex.glsl",    // shaderをインスタンス化
                      "shaders/fragment.glsl");

    // .objファイルから頂点データを取得
    std::vector<float>                     vertices = tokenizer.getVertices();
    // std::vector<float>                  vertexUVs = tokenizer.getVertexUVs();
    std::vector<std::vector<unsigned int>> faces = tokenizer.getFaces();
  
    Object  obj(vertices, faces);               // VAO,VBO,EBO設定
    obj.setupGPU();
    Texture texture("img/wall.jpg");            // texture読み込み
  
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
    // レンダリングループ
    while (!glfwWindowShouldClose(window.getWindow()))
    {
      window.processInput();                    // IO
  
      // update
  
      // render
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
      // glBindTexture(GL_TEXTURE_2D, texture);
      texture.bind();

      // CPU側で変換行列を作る
      Mat4  model;
      model *= Mat4::rotate(Mat4::radians(-90.0f), Vec3(0.0f, 1.0f, 0.0f));
      model *= Mat4::rotate((float)glfwGetTime() , Vec3(-1.0f, 1.0f, 1.0f));
      model *= Mat4::scale(0.4f, 0.4f, 0.4f);
      Vec3  center = tokenizer.getCenter();
      model *= Mat4::translate(-center.x, -center.y, -center.z);
      Mat4  view;
      view *= Mat4::translate(0.0f, 0.0f, -3.0f);
      Mat4  projection;
      projection *= Mat4::perspective(Mat4::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
      
      // GPUに渡す
      // 使用するshaderprogramを指定し、作った行列をuniformに送る
      ourShader.use();
      ourShader.setMat4("model", model);
      ourShader.setMat4("view", view);
      ourShader.setMat4("projection", projection);
  
      // 描画
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // mode設定
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // mode設定
      obj.draw();
  
      // swap buffers and poll IO events
      glfwSwapBuffers(window.getWindow());
      glfwPollEvents();
    }

  } // obj→shader→windowの順番でデストラクタが呼ばれる

  // glfw: terminate, clean all GLFW resources
	glfwTerminate();

	return 0;
}
