#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "utils/utils.hpp"
#include "parser/Tokenizer.hpp"
#include "gl/Shader.hpp"
#include "gl/Window.hpp"
#include "gl/Object.hpp"
#include "gl/Texture.hpp"
#include "gl/Manipulator.hpp"
#include "gl/Camera.hpp"
#include "gl/Instance.hpp"
#include "math/Vec3.hpp"
#include "validate/MeshDiagnostics.hpp"

int main(int argc, char **argv)
{
  std::srand(static_cast<unsigned int>(std::time(NULL)));
  Utils::checkArgv(argc, argv);
  Tokenizer tokenizer(argv[1]);

  MeshDiagnostics diagnostics(tokenizer.getVertices(),
                              tokenizer.getFaces());
  diagnostics.checkAll();
  
  // window作成とOpenGL初期設定
  Window window;
  glEnable(GL_DEPTH_TEST);                            // 深度テストを有効にする
  glEnable(GL_BLEND);                                 // ブレンディングを有効にする
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // どうブレンドするかを指定する
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);               // 背景色   
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);          // 描画mode設定
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDisable(GL_CULL_FACE);
  
  // textureを読み込み、OpenGL textureを作成
  Texture texture(argv[2]);
  float   textureScale = 1.0f;
  bool    useTexture = false;
   
  // objのデータ作成  VAO,VBO,bounds,uv面設定
  Object  obj(tokenizer.getVertices(),
              tokenizer.getFaces(),
              tokenizer.getBounds(),
              Object::UV_YZ);
  obj.setupGPU();
  
  // objを初期ポジションに置く
  Vec3  center = tokenizer.getCenter();
  Manipulator manipulator(center);
  
  // camera作成
  Camera  camera;

  // shaderをインスタンス化
  Shader  shader("shaders/vertex.glsl", "shaders/fragment.glsl");

  std::vector<Instance> instances;
  Instance::setInstance(instances);
  bool  addInstance;
  bool  resetInstances;

  // レンダリングループ 
  while (!window.shouldClose())
  {
    // キー操作
    window.processInput(manipulator, useTexture, textureScale,
                        addInstance, resetInstances);
    if (addInstance)
      Instance::setInstance(instances);
    if (resetInstances && instances.size() > 1)
      instances.erase(instances.begin() + 1, instances.end());

    // 毎フレーム画面と深度バッファをクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture.bind();

    // GPUに渡す / 使用するshaderprogramを指定し、作った行列をuniformに送る
    shader.use();
    shader.setBool("useTexture", useTexture);
    shader.setFloat("textureScale", textureScale);
    // CPU側で変換行列を作て渡す modelは作りながら渡すので最後
    Mat4  view = camera.getViewMatrix();
    Mat4  projection = camera.getProjectionMatrix(window.getAspectRatio());
    for (std::size_t i = 0; i < instances.size(); ++i)
    {
      const Instance& instance = instances[i];
      
      Mat4  model = manipulator.getModelMatrix(instance);
      shader.setMatrices(model, view, projection);
      // 描画
      obj.draw();
    }

    // swap buffers and poll IO events
    glfwSwapBuffers(window.getWindow());
    glfwPollEvents();
  }
 
	return 0;
}


