#include "gl/Camera.hpp"

Camera::Camera()
  : distance_(Common::CAMERA_DISTANCE),
    fov_(Common::CAMERA_FOV),
    near_(Common::CAMERA_NEAR),
    far_(Common::CAMERA_FAR)
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
