#pragma once

#include "math/Vec3.hpp"

#include <vector>

class MeshDiagnostics {
private:
  const std::vector<float>&                       vertices_;
  const std::vector<std::vector<unsigned int> >&  faces_;

  Vec3  getVertex(unsigned int index) const;
  void  checkAreaSize() const;
  void  checkFaceWinding() const;     // faceを使って辺の状態をチェックする
  void  checkGeometricEdges() const;  // 頂点座標を使って辺の状態をチェックする
  void  checkTriangleShape() const;   // 三角の形状をチェックする
  void  checkFaceCoplanarity() const; // faceの頂点が同一平面にあるかをチェックする

public:
  MeshDiagnostics(const std::vector<float>& vertices,
                  const std::vector<std::vector<unsigned int> >& faces);
  ~MeshDiagnostics();

  void  checkAll() const;
};
