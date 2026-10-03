#pragma once

#include <glad/glad.h>
#include <vector>

#include "Common.hpp"

class Object {
public:
  enum UVMode {
    UV_XY,
    UV_YZ,
    UV_ZX
  };

  private:
  std::vector<float>                      vertices_;
  std::vector<std::vector<unsigned int>>  faces_;
  std::vector<float>                      vertexData_;  // 色情報を入れたvertexデータ
  Common::Bounds                          bounds_;
  UVMode                                  uvMode_;      // uv座標軸

  unsigned int  VAO_;
  unsigned int  VBO_;
  
  void  setVertexData();
  void  addVertexData(unsigned int index, float gray);
  
public:
  Object(const std::vector<float>& vertices,
         const std::vector<std::vector<unsigned int> >& faces,
         const Common::Bounds& bounds,
         UVMode mode = UV_YZ);
  ~Object();
  Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;

  void  setupGPU();
  void  setUVMode(UVMode mode);
  void  draw() const;
};

// setupGPU()の担当範囲
// VAO作成
//  ↓
// VBO作成
//  ↓
// vertices_ → vertexData_ →　VBO
//  ↓
// 頂点属性の配線
//  ↓
// VAOに設定を記憶させる