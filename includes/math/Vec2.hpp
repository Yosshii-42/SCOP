#pragma once

#include <stdexcept>
#include <ostream>

class Vec2 {
public:
  float x;
  float y;

  Vec2();
  Vec2(float x, float y);
  ~Vec2();

  // operators
  Vec2  operator+(const Vec2& right) const;
  Vec2  operator-(const Vec2& right) const;
  Vec2  operator*(float scalar) const;
  Vec2  operator/(float scalar) const;
  Vec2& operator+=(const Vec2& right);
  Vec2& operator-=(const Vec2& right);
  Vec2& operator*=(float scalar);
  Vec2& operator/=(float scalar);
};

std::ostream& operator<<(std::ostream& out, const Vec2& vec);
