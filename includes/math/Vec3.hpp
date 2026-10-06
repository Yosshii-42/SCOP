#pragma once

// #include <stdexcept>
#include <ostream>

class Vec3 {
public:
  float x;
  float y;
  float z;

  Vec3();
  Vec3(float x, float y, float z);
  ~Vec3();

  // operators
  Vec3  operator+(const Vec3& right) const;
  Vec3  operator-(const Vec3& right) const;
  Vec3  operator*(float scalar) const;
  Vec3  operator/(float scalar) const;
  Vec3& operator+=(const Vec3& right);
  Vec3& operator-=(const Vec3& right);
  Vec3& operator*=(float scalar);
  Vec3& operator/=(float scalar);
  bool  operator==(const Vec3& right) const;
  bool  operator!=(const Vec3& right) const;

  // functions
  float length() const;
  Vec3  normalize() const;
  float dot(const Vec3& right) const;
  Vec3  cross(const Vec3& right) const;
};

// overload
std::ostream& operator<<(std::ostream& out, const Vec3& vec);
