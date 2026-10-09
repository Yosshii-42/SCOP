#pragma once

#include <ostream>

class Vec2 {
public:
  float x;
  float y;

  Vec2();
  Vec2(float x, float y);
  ~Vec2();

  // operator
  Vec2  operator-(const Vec2& right) const;

  // functions
  float dot(const Vec2& right) const;
  float cross(const Vec2& right) const;
  static float getRotateDirection(const std::vector<Vec2>& vertex2D);
  bool  judgeTriangle(const Vec2& a, const Vec2& b, const Vec2& c) const;
};

// overload
std::ostream& operator<<(std::ostream& out, const Vec2& vec);
