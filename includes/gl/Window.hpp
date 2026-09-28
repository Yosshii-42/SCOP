#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Window {
private:
  GLFWwindow* window_;

  static void framebufferSizeCallback(
    GLFWwindow* window, int width, int height);

  Window(const Window&);
  Window& operator=(const Window&);

public:
  Window();
  ~Window();

  GLFWwindow* getWindow() const;
  void processInput();
};
