#include "gl/Operation.hpp"

Operation::Operation(const Vec3& center)
  : center_(center),
    rotation_(0.0f, 0.0f, 0.0f),
    position_(0.0f, 0.0f, 0.0f),
    scale_(1.0f, 1.0f, 1.0f)
{}

Operation::~Operation() {}

void  Operation::rotateX(float angle)
{
  rotation_.x += angle;
}

void  Operation::rotateY(float angle)
{
  rotation_.y += angle;
}

void  Operation::rotateZ(float angle)
{
  rotation_.z += angle;;
}

void  Operation::translateX(float trans)
{
  position_.x += trans;
}

void  Operation::translateY(float trans)
{
  position_.y += trans;
}

void  Operation::translateZ(float trans)
{
  position_.z += trans;
}

void  Operation::scale(float scale)
{
  scale_ *= scale;
}

// scale, rotate, translateの順番でmodel用Mat4を作成する
Mat4  Operation::getModelMatrix() const
{
  Mat4  model;

  // 最後に適用したい translate(移動) を最初に書く
  model *= Mat4::translate(position_.x, position_.y, position_.z);

  // rotate
  model *= Mat4::rotate(rotation_.x, Vec3(1.0f, 0.0f, 0.0f));
  model *= Mat4::rotate(rotation_.y, Vec3(0.0f, 1.0f, 0.0f));
  model *= Mat4::rotate(rotation_.z, Vec3(0.0f, 0.0f, 1.0f));

  // 最初に適用したい　scale を最後に書く
  model *= Mat4::scale(scale_.x, scale_.y, scale_.z);

  // center補正
  model *= Mat4::translate(-center_.x, -center_.y, -center_.z);

  return (model);
}
