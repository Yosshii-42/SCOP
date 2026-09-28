#include "gl/Window.hpp"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Window::Window() : window_(NULL)
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
  window_ = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window_ == NULL)
	{
		glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_, Window::framebufferSizeCallback);

	// glad: OpenGL関数を呼び出す前にGLADを初期化する
  // -----------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
    glfwDestroyWindow(window_);
    glfwTerminate();
    throw std::runtime_error("Failed to initialize GLAD");
	}

}

Window::~Window() 
{
  if (window_ != NULL)
    glfwDestroyWindow(window_);
  glfwTerminate();
}

GLFWwindow*  Window::getWindow() const
{
  return (window_);
}

void Window::processInput()
{
	if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window_, true);
}

// ウィンドウのサイズが変更されるたびに呼び出されるコールバック関数
void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	(void)window;

  // OpenGLにレンダリングウィンドウのサイズを伝える
	// 最初の２つの引数でウィンドウの左下隅の位置を設定する
	glViewport(0, 0, width, height);
}
