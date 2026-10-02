#include "gl/Window.hpp"

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
  window_ = glfwCreateWindow(Common::SCR_WIDTH,
                             Common::SCR_HEIGHT,
                             "LearnOpenGL",
                             NULL,
                             NULL);
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

void Window::processInput(Operation& operation)
{
  // esc
	if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window_, true);

  // [→] 右回転
  if (glfwGetKey(window_, GLFW_KEY_RIGHT) == GLFW_PRESS)
    operation.rotateY(Mat4::radians(1.0f));
  // [←] 左回転
  if (glfwGetKey(window_, GLFW_KEY_LEFT ) == GLFW_PRESS)
    operation.rotateY(Mat4::radians(-1.0f));
  // [↑] x軸上回転
  if (glfwGetKey(window_, GLFW_KEY_UP) == GLFW_PRESS)
    operation.rotateX(Mat4::radians(-1.0f));
  // [↓] x軸下回転
  if (glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS)
    operation.rotateX(Mat4::radians(1.0f));
  // [Q] z軸右回転
  if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS)
    operation.rotateZ(Mat4::radians(1.0f));
  // [E] z軸左回転
  if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS)
    operation.rotateZ(Mat4::radians(-1.0f));
  
  // [R] 上移動
  if (glfwGetKey(window_, GLFW_KEY_R) == GLFW_PRESS)
    operation.translateY(0.01f);
  // [F] 下移動
  if (glfwGetKey(window_, GLFW_KEY_F) == GLFW_PRESS)
    operation.translateY(-0.01f);
  // [A] 左移動
  if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
    operation.translateX(-0.01f);
  // [D] 右移動
  if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
    operation.translateX(0.01f);
  // [W] 前へ移動
  if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
    operation.translateZ(0.01f);
  // [S] 後ろへ移動
  if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
    operation.translateZ(-0.01f);

  // [Z] 拡大
  if (glfwGetKey(window_, GLFW_KEY_Z) == GLFW_PRESS)
    operation.scale(1.01f);
  // [X] 縮小
  if (glfwGetKey(window_, GLFW_KEY_X) == GLFW_PRESS)
    operation.scale(0.99f);
}

// ウィンドウのサイズが変更されるたびに呼び出されるコールバック関数
void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	(void)window;

  // OpenGLにレンダリングウィンドウのサイズを伝える
	// 最初の２つの引数でウィンドウの左下隅の位置を設定する
	glViewport(0, 0, width, height);
}
