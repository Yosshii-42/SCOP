#include "validate/MeshDiagnostics.hpp"

#include <iostream>
#include <map>
#include <utility>
#include <algorithm>
#include <tuple>
#include <cmath>

MeshDiagnostics::MeshDiagnostics(const std::vector<float>& vertices,
                                 const std::vector<std::vector<unsigned int> >& faces)
  : vertices_(vertices), faces_(faces)
{}

MeshDiagnostics::~MeshDiagnostics() {}

// face[i]の頂点座標をVec3で得る
Vec3  MeshDiagnostics::getVertex(unsigned int index) const
{
  return (Vec3(vertices_[index * 3],
               vertices_[index * 3 + 1],
               vertices_[index * 3 + 2]
              ));
}

void  MeshDiagnostics::checkAreaSize() const
{
  std::size_t degenerateCount = 0;
  for (std::size_t i = 0; i < faces_.size(); ++i)
  {
    const std::vector<unsigned int>& face = faces_[i];

    if (face.size() != 3)
      continue;

    Vec3  a = getVertex(face[0] - 1);
    Vec3  b = getVertex(face[1] - 1);
    Vec3  c = getVertex(face[2] - 1);

    Vec3  cross = (b - a).cross(c - a);

    if (cross.length() < 1e-6f)
    {
      ++degenerateCount;
      std::cout << "Degenerate face: " << i << std::endl;
    }
  }
  std::cout << "[Check Area Size]" << std::endl;
  std::cout << "Degenerate triangles: " << degenerateCount << std::endl;
  std::cout << std::endl;
}

void  MeshDiagnostics::checkFaceWinding() const
{
  typedef std::pair<unsigned int, unsigned int> Edge;

  // key: 変を構成する2頂点
  // value.first: その辺が使われた回数
  // value.second: 辺の向きの合計
  std::map<Edge, std::pair<int, int> > edges;
  int zeroLengthEdges = 0;

  for (size_t i = 0; i < faces_.size(); ++i)
  {
    const std::vector<unsigned int>& face = faces_[i];

    for (size_t j = 0; j < face.size(); ++j)
    {
      unsigned int a = face[j];
      unsigned int b = face[(j + 1) % face.size()];

      if (a == b) // 連続した頂点についてのみ異常をカウント
      {
        zeroLengthEdges++;
        std::cout << "Duplicate vertex in face: " << i << std::endl;
        continue;
      }

      // キーは小さいものを先頭にする
      Edge  key(std::min(a, b), std::max(a, b));

      // a < bなら+1, a > bなら-1
      int direction = (a < b) ? 1 : -1;

      std::pair<int, int>&  info = edges[key];

      ++info.first;
      info.second += direction;
    }
  }

  size_t  boundary = 0;     // 1つの面だけに使用されている辺
  size_t  inconsistent = 0; // 向きが不整合な辺
  size_t  nonManifold = 0; // 3つ以上の面で共有されている辺

  for (std::map<Edge, std::pair<int, int> >::const_iterator it
        = edges.begin(); it != edges.end(); ++it)
  {
    int count = it->second.first;
    int directionSum = it->second.second;

    if (count == 1)
      ++boundary;
    else if (count == 2 && directionSum != 0)
      ++inconsistent;
    else if (count > 2)
      ++nonManifold;
  }

  std::cout << "[Check Face Winding]" << std::endl;
  std::cout << "Boundary edges: " << boundary << std::endl;
  std::cout << "Inconsistent edges: " << inconsistent << std::endl;
  std::cout << "Non-manifold edges: " << nonManifold << std::endl;
  std::cout << "Zero-length edges: " << zeroLengthEdges << std::endl;
  std::cout << std::endl;
}

void  MeshDiagnostics::checkGeometricEdges() const
{
  typedef std::tuple<long long, long long, long long> Point;
  typedef std::pair<Point, Point> Edge;

  const double  precision = 1000000.0;

  std::map<Edge, std::pair<int, int> >  edges;
  int zeroLengthEdges = 0;
  
  for (size_t i = 0; i < faces_.size(); ++i)
  {
    const std::vector<unsigned int>& face = faces_[i];
    for (size_t j = 0; j < face.size(); ++j)
    {
      Vec3  a = getVertex(face[j] - 1);
      Vec3  b = getVertex(face[(j + 1) % face.size()] - 1);

      // 整数値で比較するため100万倍して近くの整数に丸める
      Point pa(std::llround(a.x * precision),
               std::llround(a.y * precision),
               std::llround(a.z * precision));
      Point pb(std::llround(b.x * precision),
               std::llround(b.y * precision),
               std::llround(b.z * precision));

      if (pa == pb)
      {
        zeroLengthEdges++;
        std::cout << "Duplicate vertex in face: " << i << std::endl;
        continue;
      }

      Edge  key(std::min(pa, pb), std::max(pa, pb));
      // pa < pbなら+1, pa > pbなら-1
      int direction = (pa < pb) ? 1 : -1;

      std::pair<int, int>&  info = edges[key];

      ++info.first;
      info.second += direction;
    }
  }

  size_t  boundary = 0;     // 1つの面だけに使用されている辺
  size_t  inconsistent = 0; // 向きが不整合な辺
  size_t  nonManifold = 0; // 3つ以上の面で共有されている辺

  for (std::map<Edge, std::pair<int, int> >::const_iterator it
        = edges.begin(); it != edges.end(); ++it)
  {
    int count = it->second.first;
    int directionSum = it->second.second;

    if (count == 1)
      ++boundary;
    else if (count == 2 && directionSum != 0)
      ++inconsistent;
    else if (count > 2)
      ++nonManifold;
  }

  std::cout << "[Check Geometric Edges]" << std::endl;
  std::cout << "Boundary edges: " << boundary << std::endl;
  std::cout << "Inconsistent edges: " << inconsistent << std::endl;
  std::cout << "Non-manifold edges: " << nonManifold << std::endl;
  std::cout << "Zero-length edges: " << zeroLengthEdges << std::endl;  
  std::cout << std::endl;
}

// faceがすべて三角形であることが前提
void  MeshDiagnostics::checkTriangleShape() const
{
  size_t  thinTriangle = 0;
  float   maxRatio = 0.0f;
  size_t  maxRatioFace = 0;

  for (size_t i = 0; i < faces_.size(); ++i)
  {
    const std::vector<unsigned int>& face = faces_[i];
  
    if (face.size() != 3)
      continue;

    Vec3  a = getVertex(face[0] - 1);
    Vec3  b = getVertex(face[1] - 1);
    Vec3  c = getVertex(face[2] - 1);

    float ab = (b - a).length();
    float bc = (c - b).length();
    float ca = (a - c).length();

    float longest = std::max(ab, std::max(bc, ca));
    float shortest = std::min(ab, std::min(bc, ca));

    if (shortest == 0.0f)
      continue;

    float ratio = longest / shortest;
    
    if (ratio > 10.0f)
      ++thinTriangle;
    if (ratio > maxRatio)
    {
      maxRatio = ratio;
      maxRatioFace = i;
    }
  }

  std::cout << "[Check Triangle Shape]" << std::endl;
  std::cout << "Thin triangles (ratio > 10): " << thinTriangle << std::endl;
  std::cout << "Maximun edge ratio: " << maxRatio << std::endl;
  std::cout << "Face of maximun ratio: " << maxRatioFace << std::endl;;
  std::cout << std::endl;
}

void  MeshDiagnostics::checkFaceCoplanarity() const
{
  size_t      nonCoplanarCount = 0;
  const float epsilon = 1e-5f;

  for (size_t i = 0; i < faces_.size(); ++i)
  {
    const std::vector<unsigned int>& face = faces_[i];

    if (face.size() <= 3)
      continue;

    Vec3  a = getVertex(face[0] - 1);

    Vec3  b = getVertex(face[1] - 1);
    Vec3  c = getVertex(face[2] - 1);

    Vec3 normal = (b - a).cross(c - a);

    float normaoLength = normal.length();
    if (normaoLength < epsilon)
      continue;

    for (size_t j = 3; j < face.size(); ++j)
    {
      Vec3  p = getVertex(face[j] - 1);
      float distance = std::abs((p - a).dot(normal) / normaoLength);

      if (distance > epsilon)
      {
        ++nonCoplanarCount;
        std::cout << "Non-coplanar face: " << i << std::endl;
        break;
      }
    }
  }
  std::cout << "Non-coplanar faces: " << nonCoplanarCount << std::endl;
}

void  MeshDiagnostics::checkAll() const
{
  checkAreaSize();
  checkFaceWinding();     // faceを使って辺の状態をチェックする
  checkGeometricEdges();  // 頂点座標を使って辺の状態をチェックする
  checkTriangleShape();   // 三角の形状をチェックする
  checkFaceCoplanarity(); // faceの頂点が同一平面にあるかをチェックする 
}
