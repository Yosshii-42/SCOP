#include "math/Vec2.hpp"

Vec2::Vec2()
  : x(0.0f), y(0.0f) {
}

Vec2::Vec2(float x, float y)
  : x(x), y(y) {
}

Vec2::~Vec2() {}

// operators
Vec2  Vec2::operator+(const Vec2& right) const
{
  Vec2  result;
  result.x = this->x + right.x;
  result.y = this->y + right.y;
  return result;
}

Vec2  Vec2::operator-(const Vec2& right) const
{
  Vec2  result;
  result.x = this->x - right.x;
  result.y = this->y - right.y;
  return result;
}

Vec2  Vec2::operator*(float scalar) const
{
  Vec2  result;
  result.x = this->x * scalar;
  result.y = this->y * scalar;
  return result;
}

Vec2  Vec2::operator/(float scalar) const
{
  if (scalar == 0.0f)
    throw std::runtime_error("Vec2: division by zero");
  Vec2  result;
  result.x = this->x / scalar;
  result.y = this->y / scalar;
  return result;
}

Vec2& Vec2::operator+=(const Vec2& right)
{
  this->x += right.x;
  this->y += right.y;
  return *this;
}

Vec2& Vec2::operator-=(const Vec2& right)
{
  this->x -= right.x;
  this->y -= right.y;
  return *this;
}

Vec2& Vec2::operator*=(float scalar)
{
  this->x *= scalar;
  this->y *= scalar;
  return *this;
}

Vec2& Vec2::operator/=(float scalar)
{
  if (scalar == 0.0f)
    throw std::runtime_error("Vec2: division by zero");
  this->x /= scalar;
  this->y /= scalar;
  return *this;
}

// overload
std::ostream& operator<<(std::ostream& out, const Vec2& vec)
{
  out << "("
      << vec.x << ", "
      << vec.y << ")";
  return out;
}
