#pragma once

#include <glad/glad.h>
#include <vector>

#include "Common.hpp"
#include "math/Vec3.hpp"
#include "math/Vec2.hpp"

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
  
  void    setVertexData();

  void    addVertexData(unsigned int index, float gray, UVMode uvMode);
  std::vector<unsigned int> triangulate(const std::vector<unsigned int>& face,
                                         UVMode mode) const;
  // utils functions
  Vec3    getVertex(unsigned int index) const;
  UVMode  getUVMode(const std::vector<unsigned int>& face) const;
  static Vec2 projectTo2D(const Vec3& vertex, Object::UVMode mode);
  bool    containsPoint(const std::vector<Vec2>& points,
                        const std::vector<unsigned int>& remaining,
                        std::size_t pre,
                        std::size_t cur,
                        std::size_t next) const;
  
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