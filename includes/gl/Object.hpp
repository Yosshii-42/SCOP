#pragma once

#include <glad/glad.h>
#include <vector>
#include <cmath>

#include "Common.hpp"
#include "math/Vec3.hpp"
#include "math/Vec2.hpp"

class Object {
public:
  struct UVBasis {
    Vec3  u;
    Vec3  v;
  };

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
  Common::UVMapping                       uvMapping_;   // UV_AXIS, UV_NORMAL

  unsigned int  VAO_;
  unsigned int  VBO_;
  
  void    setVertexData();

  // 三角分割処理
  void    trianglateFan(const std::vector<unsigned int>& face,
                        float gray, UVMode mode, const UVBasis& uvBasis);
  void    triangulateEarClipping(const std::vector<unsigned int>& face,
                                 float gray, UVMode uvMode, const UVBasis& uvBasis);
  void    addVertexData(unsigned int index, float gray,
                        UVMode uvMode, const UVBasis& uvBasis);
  std::vector<unsigned int> triangulate(const std::vector<unsigned int>& face,
                                         UVMode uvMode) const;
  // utils functions
  Vec3    getVertex(unsigned int index) const;
  UVMode  getUVMode(const std::vector<unsigned int>& face, Vec3& normal) const;
  // 軸、または法線を返す
  Vec2    getUVAxis(const Vec3& p, UVMode mode) const;
  Vec2    getUVNormal(const Vec3& p, const UVBasis& basis) const;
  // normalを受け取って、Vec3 u, Vec3 vを返す
  UVBasis getUVBasis(const Vec3& normal) const;
  static Vec2 projectTo2D(const Vec3& vertex, Object::UVMode uvMode);
  bool    containsPoint(const std::vector<Vec2>& points,
                        const std::vector<unsigned int>& remaining,
                        std::size_t pre,
                        std::size_t cur,
                        std::size_t next) const;
  
  public:
  Object(const std::vector<float>& vertices,
         const std::vector<std::vector<unsigned int> >& faces,
         const Common::Bounds& bounds,
         Common::UVMapping mapping = Common::UV_AXIS);
  ~Object();
  Object(const Object&) = delete;
  Object& operator=(const Object&) = delete;
  
  void  setupGPU();                               // vertexデータをGPUに渡す
  void  setUVMapping(Common::UVMapping mapping);  // uvのセット方法を変更する
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