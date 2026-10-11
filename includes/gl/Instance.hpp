#pragma once

#include "math/Vec3.hpp"

struct Instance {
  Vec3  position;
  Vec3  rotationAxis;
  float scale;
  float rotationSpeed;
  static void  setInstance(std::vector<Instance>& instances);
};
