#pragma once

namespace Common
{
  // screen settings
  const unsigned int DEFAULT_WIDTH = 800;
  const unsigned int DEFAULT_HEIGHT = 600;

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

