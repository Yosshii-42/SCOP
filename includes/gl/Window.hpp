#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Common.hpp"
#include "gl/Operation.hpp"

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
  void processInput(Operation& operation);
};
