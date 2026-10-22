#include "gl/Object.hpp"

#include <iostream>

Object::Object(const std::vector<float>& vertices,
               const std::vector<std::vector<unsigned int> >& faces,
               const Common::Bounds& bounds,
               Common::UVMapping mapping)
  : vertices_(vertices), faces_(faces), bounds_(bounds), uvMapping_(mapping), VAO_(0), VBO_(0)
{
  setVertexData();
}

Object::~Object()
{
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
}

// 頂点データを格納する
void  Object::setVertexData()
{
  for (size_t i = 0; i < faces_.size(); ++i)
  {
    const std::vector<unsigned int>& face = faces_[i];
    if (face.size() < 3)
      throw std::runtime_error("Face size error");

    // faceのUVModeとnormalを取得する
    Vec3    normal;
    UVMode  uvMode = getUVMode(face, normal);


    // color指定
    // float gray = 0.7f; // 1色
    float gray = 0.5f + (i % 4) * 0.15f; // 規則的
    // float gray = 0.3f + static_cast<float>(std::rand() % 46) / 100.0f; // ランダム

    // 頂点処理手法指定
    // trianglateFan(face, gray, uvMode, getUVBasis(normal));
    triangulateEarClipping(face, gray, uvMode, getUVBasis(normal));
  }
}

// 扇状に分割する処理
void  Object::trianglateFan(const std::vector<unsigned int>& face,
                            float gray, UVMode uvMode, const UVBasis& uvBasis)
{
  for (size_t j = 1; j + 1 < face.size(); ++j)
  {
    // 3つの頂点のface番号を取得する
    unsigned int  index1 = face[0] - 1;
    unsigned int  index2 = face[j] - 1;
    unsigned int  index3 = face[j + 1] - 1;

    // face番号を元に、3つの頂点の座興データを取得し、vertexData_にpush_backする
    addVertexData(index1, gray, uvMode, uvBasis);
    addVertexData(index2, gray, uvMode, uvBasis);
    addVertexData(index3, gray, uvMode, uvBasis);
  }
}

// Ear Clippingを使って分割する処理
void  Object::triangulateEarClipping(const std::vector<unsigned int>& face,
                                     float gray, UVMode uvMode, const UVBasis& uvBasis)
{
  // Ear Clippingで三角形に分割(凹み図形に対応する)
  std::vector<unsigned int> triangulars = triangulate(face, uvMode);
  
  for (size_t j = 0; j + 2 < triangulars.size(); j += 3)
  {
    // 3つの頂点のface番号を取得する
    unsigned int  index1 = triangulars[j] - 1;
    unsigned int  index2 = triangulars[j + 1] - 1;
    unsigned int  index3 = triangulars[j + 2] - 1;

    // face番号を元に、3つの頂点の座興データを取得し、vertexData_にpush_backする
    addVertexData(index1, gray, uvMode, uvBasis);
    addVertexData(index2, gray, uvMode, uvBasis);
    addVertexData(index3, gray, uvMode, uvBasis);
  }
}

// x,y,z,r,g,b,u,vの設定
void  Object::addVertexData(unsigned int index, float gray,
                            UVMode uvMode, const UVBasis& uvBasis)
{
  Vec3  p = getVertex(index);
  Vec2  uv;

  if (uvMapping_ == Common::UV_AXIS)
    uv = getUVAxis(p, uvMode);
  else
    uv = getUVNormal(p, uvBasis);

  vertexData_.push_back(p.x);
  vertexData_.push_back(p.y);
  vertexData_.push_back(p.z);

  vertexData_.push_back(gray);  // r
  vertexData_.push_back(gray);  // g
  vertexData_.push_back(gray);  // b
  
  vertexData_.push_back(uv.x);  // u
  vertexData_.push_back(uv.y);  // v
}

// Ear Clippingによる三角形分割を計算する
std::vector<unsigned int> Object::triangulate(const std::vector<unsigned int>& face,
                                               Object::UVMode mode) const
{
  // face(3D)->2D投影
  std::vector<Vec2> vertex2Dpoints;

  for (std::size_t i = 0; i < face.size(); ++i)
  {
    Vec3  vertex = getVertex(face[i] - 1);
    Vec2  vertex2D = projectTo2D(vertex, mode);
    vertex2Dpoints.push_back(vertex2D);
  }

  // 多角形全体の頂点の回る方向を調べる(面積で判定)
  float area = Vec2::getRotateDirection(vertex2Dpoints);

  // ear clipping
  std::vector<unsigned int> triangles;
  std::vector<unsigned int> remaining;

  // faceのindexをremainingに入れる
  for (std::size_t i = 0; i < face.size(); ++i)
    remaining.push_back(i);
  
  while (remaining.size() > 3)
  {
    bool  earFound = false;
    std::size_t size = remaining.size();
    for (std::size_t i = 0; i < size; ++i)
    {
      std::size_t pre = remaining[(i + size - 1) % size];
      std::size_t cur = remaining[i];
      std::size_t next = remaining[(i + 1) % size];
      Vec2  preVec2 = vertex2Dpoints[pre];
      Vec2  curVec2 = vertex2Dpoints[cur];
      Vec2  nextVec2 = vertex2Dpoints[next];
      float  unevenness;

      // 凹凸判定 凸角の場合はスキップ
      unevenness = (curVec2 - preVec2).cross(nextVec2 - curVec2);
      if (unevenness * area <= 0.0f) // 逆回転の場合はスキップ
        continue;

      // 三角形内に他の頂点があればスキップ
      if (containsPoint(vertex2Dpoints, remaining, pre, cur, next))
        continue;

      // 三角の頂点をtrianglesに登録してcurを削除する
      triangles.push_back(face[pre]);
      triangles.push_back(face[cur]);
      triangles.push_back(face[next]);

      remaining.erase(remaining.begin() + i);

      earFound = true;
      break ;
    }
    if (!earFound)
      throw std::runtime_error("Ear clipping failed");
  }
  // 残った3点をpush_backする
  triangles.push_back(face[remaining[0]]);
  triangles.push_back(face[remaining[1]]);
  triangles.push_back(face[remaining[2]]);

  return (triangles);
}

// face[i]の頂点座標をVec3で得る
Vec3  Object::getVertex(unsigned int index) const
{
  return (Vec3(vertices_[index * 3],
               vertices_[index * 3 + 1],
               vertices_[index * 3 + 2]
              ));
}

// faceの代表法線と投影面を決定
Object::UVMode  Object::getUVMode(const std::vector<unsigned int>& face,
                                  Vec3& normal) const
{
  // faces_[i]の法線ベクトルを求める Newell's Method
  for (std::size_t i = 0; i < face.size(); ++i)
  {
    Vec3  a = getVertex(face[i] - 1);
    Vec3  b = getVertex(face[(i + 1) % face.size()] - 1);

    normal.x += (a.y - b.y) * (a.z + b.z);
    normal.y += (a.z - b.z) * (a.x + b.x);
    normal.z += (a.x - b.x) * (a.y + b.y);
  }
  if (normal.length() == 0.0f)
    throw std::runtime_error("Invalid face normal: zero-length vector");
  normal = normal.normalize();

  // x, y, zの最大と成分を求める
  float absX = std::fabs(normal.x);
  float absY = std::fabs(normal.y);
  float absZ = std::fabs(normal.z);

  // UV面を決める
  if (absX >= absY && absX >= absZ)
    return (UV_YZ);
  else if (absY >= absX && absY >= absZ)
    return (UV_ZX);
  else
    return (UV_XY);
}

// 軸貼り付け u,vを求める
Vec2  Object::getUVAxis(const Vec3& p, Object::UVMode uvMode) const
{
  float u;
  float v;
  
  switch (uvMode)
  {
    case UV_XY:
      u = p.x - bounds_.minX; // textureの(0,0)を元にした座標を求める
      v = p.y - bounds_.minY;
      break;
    
    case UV_YZ:
      v = p.y - bounds_.minY;
      u = p.z - bounds_.minZ;
      break;

    case UV_ZX:
      v = p.z - bounds_.minZ;
      u = p.x - bounds_.minX;
      break;
  }
  return (Vec2(u, v));
}

// 法線貼り付け u,vを求める
Vec2  Object::getUVNormal(const Vec3& p, const UVBasis& basis) const
{
  float u = p.dot(basis.u);
  float v = p.dot(basis.v);
  return (Vec2(u, v));
}

// 法線からUVの基底ベクトルを生成 Vec3 u, Vec3 v の構造体を返す
Object::UVBasis Object::getUVBasis(const Vec3& normal) const
{
  Vec3  helperAxis;

  if (std::fabs(normal.y) < 0.9f)
    helperAxis = Vec3(0.0f, 1.0f, 0.0f);
  else
    helperAxis = Vec3(1.0f, 0.0f, 0.0f);

  Vec3  u = helperAxis.cross(normal).normalize();
  Vec3  v = normal.cross(u);

  Object::UVBasis uvBasis;
  uvBasis.u = u;
  uvBasis.v = v;
  return (uvBasis);  
} 

// 3D->2D投影
Vec2 Object::projectTo2D(const Vec3& vertex, Object::UVMode mode)
{
  if (mode == Object::UV_XY)
    return (Vec2(vertex.x, vertex.y));
  else if (mode == Object::UV_YZ)
    return (Vec2(vertex.y, vertex.z));
  else
    return (Vec2(vertex.z, vertex.x));
}

// 頂点が三角形内にあるかの判定
bool  Object::containsPoint(const std::vector<Vec2>& points,
                        const std::vector<unsigned int>& remaining,
                        std::size_t pre,
                        std::size_t cur,
                        std::size_t next) const
{
  Vec2  preVec2 = points[pre];
  Vec2  curVec2 = points[cur];
  Vec2  nextVec2 = points[next];
  for (size_t j = 0; j < remaining.size(); ++ j)
  {
    std::size_t index = remaining[j];
    if (index == pre || index == cur || index == next)
      continue;

    const Vec2& p = points[index];
    if (p.judgeTriangle(preVec2, curVec2, nextVec2))
      return (true); 
  }
  return (false);
}

// public
// GPUにデータを渡す
void  Object::setupGPU()
{
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);

  // bind the vertex array object first, then bind and adt vertex buffers, and then cofigure certex attributess.
  glBindVertexArray(VAO_);

  // 頂点データ
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER,
              vertexData_.size() * sizeof(float),
              vertexData_.data(),
              GL_STATIC_DRAW);
  // glBufferData(GL_ARRAY_BUFFER, vertexUVs.size() * sizeof(float), vertexUVs.data(), GL_STATIC_DRAW);

  // 頂点データをどのように解釈すべきかを指示
  // location = 0 = position
  glVertexAttribPointer(0,                  // location = 0
                        3,                  // x, y, z の3要素
                        GL_FLOAT,
                        GL_FALSE,
                        8 * sizeof(float),  // 次の頂点まで8 float
                        (void*)0);          // 先頭から読む
  glEnableVertexAttribArray(0);

  // location = 1 = color
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // location = 2 = uv座標
  glVertexAttribPointer(2,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        8 * sizeof(float),
                        (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // unbind VBO_
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // unbind VAO_
  glBindVertexArray(0);
}

// mappingの変更に応じてVBOを更新する
void  Object::setUVMapping(Common::UVMapping mapping)
{
  if (uvMapping_ == mapping)
    return ;

  uvMapping_ = mapping;

  // 頂点データ再生成
  vertexData_.clear();
  setVertexData();

  // VBO更新
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER,
              vertexData_.size() * sizeof(float),
              vertexData_.data(),
              GL_STATIC_DRAW);
}

void  Object::draw() const
{
  glBindVertexArray(VAO_);

  glDrawArrays(GL_TRIANGLES,
               0,
               static_cast<GLsizei>(vertexData_.size() / 8));

  glBindVertexArray(0);
}
