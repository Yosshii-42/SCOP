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
  
  // input flagをインスタンス化
  Common::InputFlags  inputFlags;

  // textureを読み込み、OpenGL textureを作成
  Texture texture(argv[2]);
  float   textureScale = 1.0f;
  float   textureBlend = 0.0f;
   
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

  // Magic Pocket用Instance作成
  std::vector<Instance> instances;
  Instance::setInstance(instances);

  // レンダリングループ 
  while (!window.shouldClose())
  {
    // キー操作
    window.processInput(manipulator, textureScale, inputFlags);
    
    // Magic Pocket
    if (inputFlags.addInstance)
      Instance::setInstance(instances);
    if (inputFlags.resetInstances && instances.size() > 1)
      instances.erase(instances.begin() + 1, instances.end());

    // texture blend
    float target = inputFlags.useTexture ? 1.0f : 0.0f;
    float step = 0.02f;
    if (textureBlend < target)
      textureBlend = std::min(textureBlend + step, target);
    else
      textureBlend = std::max(textureBlend - step, target);
    texture.bind();

    // 毎フレーム画面と深度バッファをクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // GPUに渡す / 使用するshaderprogramを指定し、作った行列をuniformに送る
    shader.use();
    shader.setBool("useTexture", inputFlags.useTexture);
    shader.setFloat("textureScale", textureScale);
    shader.setFloat("textureBlend", textureBlend);
  
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


