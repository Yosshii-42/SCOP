#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sys/stat.h>

#include "parser/tokenizer.hpp"
#include "shader/Shader.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void  checkArgv(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Argument number error" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  struct stat status;

  if (stat(argv[1], &status) != 0) {
    std::cerr << "Failed to stat file: " << argv[1] << std::endl;
    std::exit(EXIT_FAILURE);
  }
  if (status.st_mode & S_IFDIR) {
    std::cerr << argv[1] << ": is a directory" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::string fileName(argv[1]);
  if (fileName.size() < 4 ||
      fileName.compare(fileName.size() - 4, 4, ".obj") != 0) {
      std::cerr << "filename must have \".obj\" extention" << std::endl;
      std::exit(EXIT_FAILURE);
  }
}

GLFWwindow*  initWindow()
{
  // reset cwd
  #ifdef __APPLE__
  glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
  #endif
  
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif


	// glfw: window creation
  // ---------------------
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: OpenGL関数を呼び出す前にGLADを初期化する
  // -----------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		std::exit(EXIT_FAILURE);
	}

  return (window);
}



int main(int argc, char **argv)
{
  checkArgv(argc, argv);
  Tokenizer tokenizer(argv[1]);
  GLFWwindow* window = initWindow();


  // 深度テストを有効にする
  glEnable(GL_DEPTH_TEST);

  // shaderをインスタンス化
  Shader  ourShader("shaders/vertex.glsl", "shaders/fragment.glsl");

  // .objファイルから頂点データを取得
  std::vector<float>            vertices = tokenizer.getVertices();
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
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  // 頂点データをどのように会社デデータをどのように解釈すべどのように解釈すべきかを指示
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // unbind VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // unbind VAO
  glBindVertexArray(0);
  
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// レンダリングループ
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// update

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // update shader uniform

    // draw triangle
    // glUseProgram(shaderProgram);
    ourShader.use();

    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

  // optional: de-allocate all resources
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  // glDeleteProgram(shaderProgram);

  // glfw: terminate, clean all GLFW resources
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// ウィンドウのサイズが変更されるたびに呼び出されるコールバック関数
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	(void)window;

  // OpenGLにレンダリングウィンドウのサイズを伝える
	// 最初の２つの引数でウィンドウの左下隅の位置を設定する
	glViewport(0, 0, width, height);
}
