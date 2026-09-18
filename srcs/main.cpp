#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sys/stat.h>

#include "utils/utils.hpp"
#include "parser/tokenizer.hpp"
#include "shader/Shader.hpp"
#include "math/Mat4.hpp"
#include "math/Vec3.hpp"
#include "math/Vec4.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

int main(int argc, char **argv)
{
  Utils::checkArgv(argc, argv);
  Tokenizer tokenizer(argv[1]);
  GLFWwindow* window = Utils::initWindow();

  // 深度テストを有効にする
  glEnable(GL_DEPTH_TEST);

  // shaderをインスタンス化
  Shader  ourShader("shaders/vertex.glsl", "shaders/fragment.glsl");

  // .objファイルから頂点データを取得
  // std::vector<float>            vertices = tokenizer.getVertices();
  std::vector<float>            vertexUVs = tokenizer.getVertexUVs();
  std::vector<std::vector<unsigned int>> faces = tokenizer.getFaces();
  std::vector<unsigned int>     indices;

  for (size_t i = 0; i < faces.size(); i++) {
    const std::vector<unsigned int>& face = faces[i];
    if (face.size() < 3)
      std::exit(EXIT_FAILURE);
    for (size_t j = 1; j + 1 < face.size(); j++) {
      indices.push_back(face[0] - 1);
      indices.push_back(face[j] - 1);
      indices.push_back(face[j + 1] - 1);
    }
  }

  unsigned int  VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // bind the vertex array object first, then bind and adt vertex buffers, and then cofigure certex attributess.
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexUVs.size() * sizeof(float), vertexUVs.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  // 頂点データをどのように解釈すべどのように解釈すべきかを指示
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // テクスチャ画像読み込み
  unsigned int  texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping/filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // loat and generate the texture
  int width, height, nrChannels;
  unsigned char *data = stbi_load("img/wall.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  // unbind VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // unbind VAO
  glBindVertexArray(0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// レンダリングループ
	while (!glfwWindowShouldClose(window))
	{
		// input
		Utils::processInput(window);

		// update

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);

    // CPU側で変換行列を作る
    Mat4  model;
    model *= Mat4::rotate(Mat4::radians(-90.0f), Vec3(0.0f, 1.0f, 0.0f));
    model *= Mat4::rotate((float)glfwGetTime() , Vec3(-1.0f, 1.0f, 0.0f));
    model *= Mat4::scale(0.6f, 0.6f, 0.6f);
    Vec3  center = tokenizer.getCenter();
    model *= Mat4::translate(-center.x, -center.y, -center.z);
    Mat4  view;
    view *= Mat4::translate(0.0f, 0.0f, -3.0f);
    Mat4  projection;
    projection *= Mat4::perspective(Mat4::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    // GPUに渡す
    // 使用するshaderprogramを指定する
    ourShader.use();
    // 作った行列をuniformに送る
    ourShader.setMat4("model", model);
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);

    
    // 描画
    glBindVertexArray(VAO);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

  // optional: de-allocate all resources
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  // glfw: terminate, clean all GLFW resources
	glfwTerminate();

	return 0;
}

// void processInput(GLFWwindow *window)
// {
// 	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
// 		glfwSetWindowShouldClose(window, true);
// }

// // ウィンドウのサイズが変更されるたびに呼び出されるコールバック関数
// void framebuffer_size_callback(GLFWwindow *window, int width, int height)
// {
// 	(void)window;

//   // OpenGLにレンダリングウィンドウのサイズを伝える
// 	// 最初の２つの引数でウィンドウの左下隅の位置を設定する
// 	glViewport(0, 0, width, height);
// }
