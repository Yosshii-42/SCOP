#pragma once

#include "math/Mat4.hpp"
#include "math/Vec4.hpp"
#include "math/Vec3.hpp"

class Operation {
private:
  Vec3  center_;
  Vec3  rotation_;
  Vec3  position_;
  Vec3  scale_;

public:
  Operation(const Vec3& center);
  ~Operation();

  void  rotateX(float angle);
  void  rotateY(float angle);
  void  rotateZ(float angle);

  void  translateX(float trans);
  void  translateY(float trans);
  void  translateZ(float trans);

  void  scale(float scale);

  Mat4  getModelMatrix() const;
};
