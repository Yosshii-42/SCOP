#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sys/stat.h>

namespace Utils
{
  void  checkArgv(int argc, char **argv);
  GLFWwindow*  initWindow();
  void processInput(GLFWwindow *window);
  void framebuffer_size_callback(GLFWwindow *window, int width, int height);
  
} // namespace Utils

