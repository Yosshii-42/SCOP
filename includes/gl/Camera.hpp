#pragma once

#include "math/Mat4.hpp"

class Camera {
private:
  float distance_;
  float fov_;
  float near_;
  float far_;

public:
  Camera();
  ~Camera();
  Camera(const Camera& other) = delete;
  Camera& operator=(const Camera& other) = delete;

  Mat4  getViewMatrix() const;
  Mat4  getProjectionMatrix(float aspectRatio) const;
};
