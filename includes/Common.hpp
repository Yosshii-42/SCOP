#pragma once

namespace Common
{
  // screen settings
  constexpr unsigned int DEFAULT_WIDTH = 800;
  constexpr unsigned int DEFAULT_HEIGHT = 600;

  // camera settings
  constexpr float CAMERA_DISTANCE = 3.0f;
  constexpr float CAMERA_FOV = 45.0f;
  constexpr float CAMERA_NEAR = 0.1f;
  constexpr float CAMERA_FAR = 100.0f;

  constexpr float VECTOR_EPSILION = 1e-5f;

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

// constexprとは
// コンパイル時に評価する
// コンパイル時に確定する定数を表す
// 対象は、変数、関数、クラスのコンストラクター