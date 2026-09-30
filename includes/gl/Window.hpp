#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Window {
private:
  GLFWwindow* window_;

  static void framebufferSizeCallback(
    GLFWwindow* window, int width, int height);
    
public:
  Window();
  ~Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  
  GLFWwindow* getWindow() const;
  void processInput();
};
