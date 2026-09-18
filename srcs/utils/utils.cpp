#include "utils/utils.hpp"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void  Utils::checkArgv(int argc, char **argv) {
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

GLFWwindow*  Utils::initWindow()
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
  glfwSetFramebufferSizeCallback(window, Utils::framebuffer_size_callback);

	// glad: OpenGL関数を呼び出す前にGLADを初期化する
  // -----------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		std::exit(EXIT_FAILURE);
	}

  return (window);
}

void Utils::processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// ウィンドウのサイズが変更されるたびに呼び出されるコールバック関数
void Utils::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	(void)window;

  // OpenGLにレンダリングウィンドウのサイズを伝える
	// 最初の２つの引数でウィンドウの左下隅の位置を設定する
	glViewport(0, 0, width, height);
}
