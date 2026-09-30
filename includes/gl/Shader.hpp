#pragma once

#include <glad/glad.h>

#include "math/Vec2.hpp"
#include "math/Vec3.hpp"
#include "math/Mat4.hpp"
#include "math/Vec4.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
private:
  unsigned int  ID_;

  void  checkCompileErrors(unsigned int shader, const std::string& type);

public:
  Shader(const char* vertexPath, const char* fragmentPath);
  ~Shader();
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  // activate function
  void  use();

  // set functions
  void  setBool(const std::string& name, bool value) const;
  void  setInt(const std::string& name, int value) const;
  void  setFloat(const std::string& name, float value) const;
  void  setVec2(const std::string& name, const Vec2& vec)const;
  void  setVec2(const std::string& name, float x, float y) const;
  void  setVec3(const std::string& name, const Vec3& vec) const;
  void  setVec3(const std::string& name, float x, float y, float z) const;
  void  setVec4(const std::string& name, const Vec4& vec) const;
  void  setVec4(const std::string& name, float x, float y, float z, float w) const;
  void  setMat4(const std::string& name, const Mat4& mat) const;
};
