#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "utils/utils.hpp"
#include "parser/tokenizer.hpp"
#include "Common.hpp"
#include "gl/Shader.hpp"
#include "gl/Window.hpp"
#include "gl/Object.hpp"
#include "gl/Texture.hpp"
#include "gl/Operation.hpp"
#include "math/Mat4.hpp"
#include "math/Vec3.hpp"
#include "math/Vec4.hpp"

int main(int argc, char **argv)
{
  Utils::checkArgv(argc, argv);
  Tokenizer tokenizer(argv[1]);
  
  {
    // window作成
    Window window;

    // 深度テストを有効にする
    glEnable(GL_DEPTH_TEST);

    // shaderをインスタンス化
    Shader  ourShader("shaders/vertex.glsl",
                      "shaders/fragment.glsl");

    // VAO,VBO,bounds,uv面設定
    Object  obj(tokenizer.getVertices(),
                tokenizer.getFaces(),
                tokenizer.getBounds(),
                Object::UV_YZ);
    obj.setupGPU();

    // texture読み込み
    Texture texture("img/wall.jpg");

    // 物体を初期ポジションに置く
    Vec3  center = tokenizer.getCenter();
    Operation operation(center);
    operation.scale(0.3f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
    bool  useTexture = false;

    // レンダリングループ
    while (!glfwWindowShouldClose(window.getWindow()))
    {
      // キー操作
      window.processInput(operation, useTexture);
  
      // 背景色
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
      texture.bind();

      // CPU側で変換行列を作る
      Mat4  model = operation.getModelMatrix();
      Mat4  view;
      view *= Mat4::translate(0.0f, 0.0f, -3.0f);
      Mat4  projection;
      projection *= Mat4::perspective(Mat4::radians(45.0f),
                                      static_cast<float>(Common::SCR_WIDTH)
                                        / static_cast<float>(Common::SCR_HEIGHT),
                                      0.1f,
                                      100.0f);
      
      // GPUに渡す / 使用するshaderprogramを指定し、作った行列をuniformに送る
      ourShader.use();
      ourShader.setMat4("model", model);
      ourShader.setMat4("view", view);
      ourShader.setMat4("projection", projection);
      ourShader.setBool("useTexture", useTexture);
  
      // 描画mode設定
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      obj.draw();
  
      // swap buffers and poll IO events
      glfwSwapBuffers(window.getWindow());
      glfwPollEvents();
    }

  } // Operation->Texture->Object->Shader->Windlwの順番でデストラクタが呼ばれる

  // glfw: terminate, clean all GLFW resources
	glfwTerminate();

	return 0;
}
