#pragma once

namespace Common
{
  // screen settings
  const unsigned int DEFAULT_WIDTH = 800;
  const unsigned int DEFAULT_HEIGHT = 600;

  // camera settings
  const float CAMERA_DISTANCE = 3.0f;
  const float CAMERA_FOV = 45.0f;
  const float CAMERA_NEAR = 0.1f;
  const float CAMERA_FAR = 100.0f;

  struct Bounds
  {
    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;
  };
}

