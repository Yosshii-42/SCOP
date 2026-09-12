#pragma once

#include <stdexcept>
#include <ostream>

class Vec4 {
public:
  float x;
  float y;
  float z;
  float w;

  Vec4();
  Vec4(float x, float y, float z, float w);
  ~Vec4();

  Vec4  operator+(const Vec4& right) const;
  Vec4  operator-(const Vec4& right) const;
  Vec4  operator*(float scalar) const;
  Vec4  operator/(float scalar) const;
  Vec4& operator+=(const Vec4& right);
  Vec4& operator-=(const Vec4& right);
  Vec4& operator*=(float scalar);
  Vec4& operator/=(float scalar);
  bool  operator==(const Vec4& right) const;
  bool  operator!=(const Vec4& right) const;
};

std::ostream& operator<<(std::ostream& out, const Vec4& vec);
