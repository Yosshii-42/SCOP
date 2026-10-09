#include "math/Vec2.hpp"

Vec2::Vec2()
  : x(0.0f), y(0.0f) {
}

Vec2::Vec2(float x, float y)
  : x(x), y(y) {
}

Vec2::~Vec2() {}

// operator
Vec2  Vec2::operator-(const Vec2& right) const {
  return Vec2(this->x - right.x, this->y - right.y);
}

float Vec2::dot(const Vec2& right) const {
  return (this->x * right.x +
          this->y * right.y);
}

float Vec2::cross(const Vec2& right) const {
  return (this->x * right.y -
          this->y * right.x);
}

// 回転方向を調べる
float Vec2::getRotateDirection(const std::vector<Vec2>& vertex2D)
{
  float       area = 0.0f;
  std::size_t size = vertex2D.size();

  if (size < 3)
    throw std::runtime_error("Invalid face: too few vertices");

  for(std::size_t i = 0; i < size; ++i)
  {
    const Vec2&  cur = vertex2D[i];
    const Vec2&  next = vertex2D[(i + 1) % size];

    area += cur.cross(next);
  }

  if (std::fabs(area) < 1e-6f)
    throw std::runtime_error("Invalid face: zero area");

  return (area);
}

// 三角形abc内に頂点があるかを判定
bool  Vec2::judgeTriangle(const Vec2& a, const Vec2& b, const Vec2& c) const
{
  float crossA = (c - a).cross(*this - a);
  float crossB = (b - a).cross(*this - b);
  float crossC = (b - c).cross(*this - c);
  bool  positive = (crossA > 0.0f || crossB > 0.0f || crossC > 0.0f);
  bool  negative = (crossA < 0.0f || crossB < 0.0f || crossC < 0.0f);
  return (!(positive || negative));
}

// overload
std::ostream& operator<<(std::ostream& out, const Vec2& vec) {
  out << "("
      << vec.x << ", " << vec.y
      << ")";
  return out;
}
