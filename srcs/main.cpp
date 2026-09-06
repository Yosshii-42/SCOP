#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sys/stat.h>

#include "parser/tokenizer.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// define source doce
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	// "	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"	gl_Position = vec4((aPos.z - 1.4) * 0.65, (aPos.y + 0.05) * 0.65, aPos.x * 0.65, 1.0);\n"
	"}\n\0";

const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

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

int main(int argc, char **argv)
{
  checkArgv(argc, argv);

  Tokenizer tokenizer(argv[1]);
  
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
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: OpenGL関数を呼び出す前にGLADを初期化する
  // -----------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
  // 深度テストを有効にする
  glEnable(GL_DEPTH_TEST);

  // build ajd compile shader programs
  // ---------------------------------
  // vertex shader
  unsigned int  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  //  check
  int   success;
  char  infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILAYION_FAILED\n" << infoLog << std::endl;
  }
  // fragment shader
  unsigned int  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  //  check
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  // link shader
  unsigned int  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  //  check
  glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  // delete
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // setup vertex data
  // -------------------
  // float vertices[] = {
  //   -0.5f, -0.5f, 0.0f, // left
  //   0.0f, -0.5f, 0.0f,  // right
  //   -0.25f, 0.4f, 0.0f,    // top

  //   0.0f, -0.5f, 0.0f, // left
  //   0.5f, -0.5f, 0.0f,  // right
  //   0.25f, 0.5f, 0.0f,    // top
  // };

  std::vector<float>            vertices = tokenizer.getVertices();
  std::vector<std::vector<unsigned int>> faces = tokenizer.getFaces();
  std::vector<unsigned int>     indices;

  std::cout << "data set" << std::endl;

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

	// レンダリングループ
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// update

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw triangle
    glUseProgram(shaderProgram);
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
  glDeleteProgram(shaderProgram);

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
