#include "gl/Camera.hpp"

Camera::Camera()
  : distance_(3.0f), fov_(45.0f), near_(0.1f), far_(100.0f)
{}

Camera::~Camera() {}

Mat4  Camera::getViewMatrix() const
{
  Mat4  view;
  view *= Mat4::translate(0.0f, 0.0f, -distance_);
  return (view);
}

Mat4  Camera::getProjectionMatrix(float aspectRatio) const
{
  Mat4  projection;
  projection *= Mat4::perspective(Mat4::radians(fov_),
                                  aspectRatio,
                                  near_,
                                  far_);
  return (projection);
}
