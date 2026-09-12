#include "math/Vec4.hpp"

Vec4::Vec4()
    : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {
}

Vec4::Vec4(float x, float y, float z, float w)
  : x(x), y(y), z(z), w(w) {
}

Vec4::~Vec4() {}

// operators
Vec4  Vec4::operator+(const Vec4& right) const {
  Vec4  result;
  result.x = this->x + right.x;
  result.y = this->y + right.y;
  result.z = this->z + right.z;
  result.w = this->w + right.w;
  return result;
}

Vec4  Vec4::operator-(const Vec4& right) const {
  Vec4  result;
  result.x = this->x - right.x;
  result.y = this->y - right.y;
  result.z = this->z - right.z;
  result.w = this->w - right.w;
  return result;
}

Vec4  Vec4::operator*(float scalar) const {
  Vec4  result;
  result.x = this->x * scalar;
  result.y = this->y * scalar;
  result.z = this->z * scalar;
  result.w = this->w * scalar;
  return result;
}

Vec4  Vec4::operator/(float scalar) const {
  if (scalar == 0.0f)
    throw std::runtime_error("Vec4: division by zero");
  Vec4  result;
  result.x = this->x / scalar;
  result.y = this->y / scalar;
  result.z = this->z / scalar;
  result.w = this->w / scalar;
  return result;
}

Vec4&  Vec4::operator+=(const Vec4& right) {
  this->x += right.x;
  this->y += right.y;
  this->z += right.z;
  this->w += right.w;
  return *this;
}

Vec4& Vec4::operator-=(const Vec4& right) {
  this->x -= right.x;
  this->y -= right.y;
  this->z -= right.z;
  this->w -= right.w;
  return *this;
}

Vec4& Vec4::operator*=(float scalar) {
  this->x *= scalar;
  this->y *= scalar;
  this->z *= scalar;
  this->w *= scalar;
  return *this;
}

Vec4& Vec4::operator/=(float scalar) {
  if (scalar == 0.0f)
    throw std::runtime_error("Vec4: division by zero");
  this->x /= scalar;
  this->y /= scalar;
  this->z /= scalar;
  this->w /= scalar;
  return *this;
}

// overload
std::ostream& operator<<(std::ostream& out, const Vec4& vec) {
  out << "("
      << vec.x << ", "
      << vec.y << ", "
      << vec.z << ", "
      << vec.w
      << ")";
  return out;
}
