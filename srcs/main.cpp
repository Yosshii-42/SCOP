#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "utils/utils.hpp"
#include "parser/Tokenizer.hpp"
#include "gl/Shader.hpp"
#include "gl/Window.hpp"
#include "gl/Object.hpp"
#include "gl/Texture.hpp"
#include "gl/Manipulator.hpp"
#include "gl/Camera.hpp"
#include "math/Vec3.hpp"

int main(int argc, char **argv)
{
  Utils::checkArgv(argc, argv);
  Tokenizer tokenizer(argv[1]);
  Window window;                // window作成

  // 各種初期設定
  glEnable(GL_DEPTH_TEST);                            // 深度テストを有効にする
  glEnable(GL_BLEND);                                 // ブレンディングを有効にする
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // どうブレンドするかを指定する
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);               // 背景色   
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);          // 描画mode設定

  // shaderをインスタンス化
  Shader  shader("shaders/vertex.glsl", "shaders/fragment.glsl");

  // objのデータ作成  VAO,VBO,bounds,uv面設定
  Object  obj(tokenizer.getVertices(),
              tokenizer.getFaces(),
              tokenizer.getBounds(),
              Object::UV_YZ);
  obj.setupGPU();
  
  // objを初期ポジションに置く
  Vec3  center = tokenizer.getCenter();
  Manipulator manipulator(center);
  manipulator.scale(0.3f);

  // camera作成
  Camera  camera;

  // texture読み込み
  Texture texture(argv[2]);
  float   textureScale = 1.0f;
  bool    useTexture = false;

  // レンダリングループ
  while (!window.shouldClose())
  {
    // キー操作
    window.processInput(manipulator, useTexture, textureScale);

    // 毎フレーム画面と深度バッファをクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture.bind();

    // CPU側で変換行列を作る
    Mat4  model = manipulator.getModelMatrix();
    Mat4  view = camera.getViewMatrix();
    Mat4  projection = camera.getProjectionMatrix(window.getAspectRatio());
    
    // GPUに渡す / 使用するshaderprogramを指定し、作った行列をuniformに送る
    shader.use();
    shader.setMatrices(model, view, projection);
    shader.setBool("useTexture", useTexture);
    shader.setFloat("textureScale", textureScale);

    // 描画
    obj.draw();

    // swap buffers and poll IO events
    glfwSwapBuffers(window.getWindow());
    glfwPollEvents();
  }
 
	return 0;
}

// スコープの中では、インスタンス化した逆順でデストラクタが呼ばれる
