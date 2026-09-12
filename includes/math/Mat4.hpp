#pragma once

#include "math/Vec4.hpp"

#include <cmath>
#include <ostream>

class Mat4 {
private:
  float m_[4][4];

public:
  Mat4();
  ~Mat4();

  // functions
  static Mat4   scale(float x, float y, float z);
  static float  radians(float degrees);
  static Mat4   translation(float x, float y, float z);
  static Mat4   rotationX(float angle);
  static Mat4   rotationY(float angle);
  static Mat4   rotationZ(float angle);
  const float*  data() const;

  // operators
  Mat4  operator*(const Mat4& right) const;
  Vec4  operator*(const Vec4& right) const;
  Mat4& operator*=(const Mat4& right);


  // overload
  friend std::ostream& operator<<(std::ostream& out, const Mat4& mat);
};

