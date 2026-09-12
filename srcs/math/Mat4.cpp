#include "math/Mat4.hpp"

Mat4::Mat4() {
  for (int row = 0; row < 4; ++row)
  {
    for (int col = 0; col < 4; ++col)
    {
      if (row == col)
        this->m_[row][col] = 1.0f;
      else
        this->m_[row][col] = 0.0f;
    }
  }
}

Mat4::~Mat4() {}

Mat4 Mat4::scale(float x, float y, float z)
{
  Mat4 result;

  result.m_[0][0] = x;
  result.m_[1][1] = y;
  result.m_[2][2] = z;
  
  return result;
}

float Mat4::radians(float degrees)
{
  return degrees * M_PI / 180.0f;
}

Mat4 Mat4::translation(float x, float y, float z) {
  Mat4 result;

  result.m_[0][3] = x;
  result.m_[1][3] = y;
  result.m_[2][3] = z;

  return result;
}

Mat4 Mat4::rotationX(float angle) {
  Mat4 result;

  result.m_[1][1] = std::cos(angle);
  result.m_[1][2] = -std::sin(angle);
  result.m_[2][1] = std::sin(angle);
  result.m_[2][2] = std::cos(angle);

  return  result;
}

Mat4 Mat4::rotationY(float angle) {
  Mat4 result;

  result.m_[0][0] = std::cos(angle);
  result.m_[0][2] = std::sin(angle);
  result.m_[2][0] = -std::sin(angle);
  result.m_[2][2] = std::cos(angle);

  return result;
}

Mat4 Mat4::rotationZ(float angle) {
  Mat4 result;

  result.m_[0][0] = std::cos(angle);
  result.m_[0][1] = -std::sin(angle);
  result.m_[1][0] = std::sin(angle);
  result.m_[1][1] = std::cos(angle);

  return result;
}

const float*  Mat4::data() const {
  return &this->m_[0][0];
}

// operators
Mat4 Mat4::operator*(const Mat4& right) const
{
  Mat4 result;

  for (int row = 0; row < 4; ++row)
  {
    for (int col = 0; col < 4; ++col)
    {
      result.m_[row][col] = 0.0f;
      for (int k = 0; k < 4; ++k)
        result.m_[row][col] += this->m_[row][k] * right.m_[k][col];
    }
  }

  return result;
}

Vec4  Mat4::operator*(const Vec4& right) const
{
  Vec4  result;

  result.x = this->m_[0][0] * right.x +
             this->m_[0][1] * right.y +
             this->m_[0][2] * right.z +
             this->m_[0][3] * right.w;
  result.y = this->m_[1][0] * right.x +
             this->m_[1][1] * right.y +
             this->m_[1][2] * right.z +
             this->m_[1][3] * right.w;
  result.z = this->m_[2][0] * right.x +
             this->m_[2][1] * right.y +
             this->m_[2][2] * right.z +
             this->m_[2][3] * right.w;
  result.w = this->m_[3][0] * right.x +
             this->m_[3][1] * right.y +
             this->m_[3][2] * right.z +
             this->m_[3][3] * right.w;
  return result;
}

Mat4& Mat4::operator*=(const Mat4& right)
{
  *this = *this * right;
  return *this;
}

// overload
std::ostream& operator<<(std::ostream& out, const Mat4& mat) {
  out << mat.m_[0][0] << " "
      << mat.m_[0][1] << " "
      << mat.m_[0][2] << " "
      << mat.m_[0][3] << std::endl;
  out << mat.m_[1][0] << " "
      << mat.m_[1][1] << " "
      << mat.m_[1][2] << " "
      << mat.m_[1][3] << std::endl;
  out << mat.m_[2][0] << " "
      << mat.m_[2][1] << " "
      << mat.m_[2][2] << " "
      << mat.m_[2][3] << std::endl;
  out << mat.m_[3][0] << " "
      << mat.m_[3][1] << " "
      << mat.m_[3][2] << " "
      << mat.m_[3][3];
  return out;
}