#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Common.hpp"
#include "gl/Manipulator.hpp"

class Window {
private:
  GLFWwindow* window_;
  int         width_;
  int         height_;

  struct KeyFlags {
    bool  tPressed_;
    bool  pPressed_;
    bool  kPressed_;
  };

  KeyFlags keyFlags_;

  static void framebufferSizeCallback(GLFWwindow* window,
                                      int width,
                                      int height);
    
public:
  Window();
  ~Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  
  GLFWwindow* getWindow() const;
  void  processInput(Manipulator& Manipulator,
                     float& textureScale,
                     Common::InputFlags& inputFlags);
  bool  shouldClose() const;
  float getAspectRatio() const;
};
