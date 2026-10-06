#pragma once

#include "math/Mat4.hpp"
#include "math/Vec3.hpp"

class Manipulator {
private:
  Vec3  center_;
  Vec3  rotation_;
  Vec3  position_;
  Vec3  scale_;

public:
  Manipulator(const Vec3& center);
  ~Manipulator();

  void  rotateX(float angle);
  void  rotateY(float angle);
  void  rotateZ(float angle);

  void  translateX(float trans);
  void  translateY(float trans);
  void  translateZ(float trans);

  void  scale(float scale);

  Mat4  getModelMatrix() const;
};
