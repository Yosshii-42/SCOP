#include "gl/Object.hpp"

Object::Object(const std::vector<float>& vertices,
               const std::vector<std::vector<unsigned int> >& faces,
               const Common::Bounds& bounds,
               UVMode mode)
  : vertices_(vertices), faces_(faces), bounds_(bounds), uvMode_(mode), VAO_(0), VBO_(0)
{
  setVertexData();
  (void)bounds_;
}

Object::~Object()
{
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
}

void  Object::setVertexData()
{
  for (size_t i = 0; i < faces_.size(); ++i)
  {
    const std::vector<unsigned int>& face = faces_[i];
    if (face.size() < 3)
      throw std::runtime_error("Face size error");

    // faceのUVModeを取得する
    UVMode  uvMode = getUVMode(face);

    // float gray = 0.5f + (i % 4) * 0.15f; // 規則的な模様が出る
    float gray = 0.3f + static_cast<float>(std::rand() % 46) / 100.0f; // ランダムな模様が出る

    for (size_t j = 1; j + 1 < face.size(); ++j)
    {
      // 3つの頂点のface番号を取得する
      unsigned int  index1 = face[0] - 1;
      unsigned int  index2 = face[j] - 1;
      unsigned int  index3 = face[j + 1] - 1;

      // face番号を元に、3つの頂点の座興データを取得し、vertexData_にpush_backする
      addVertexData(index1, gray, uvMode);
      addVertexData(index2, gray, uvMode);
      addVertexData(index3, gray, uvMode);
    }
  }
}

// face[i]の頂点座標をVec3で得る
Vec3  Object::getVertex(unsigned int index) const
{
  return (Vec3(vertices_[index * 3],
               vertices_[index * 3 + 1],
               vertices_[index * 3 + 2]
              ));
}

// faceのUV面を決める
Object::UVMode  Object::getUVMode(const std::vector<unsigned int>& face) const
{
  // faces_[i]の法線ベクトルを求める
  Vec3  p0 = getVertex(face[0] - 1);
  Vec3  p1 = getVertex(face[1] - 1);
  Vec3  p2 = getVertex(face[2] - 1);
  Vec3  p0_p1 = p1 - p0;
  Vec3  p0_p2 = p2 - p0;
  Vec3  normal = p0_p1.cross(p0_p2).normalize();

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

void  Object::addVertexData(unsigned int index, float gray, UVMode uvMode)
{
  float x = vertices_[index * 3];
  float y = vertices_[index * 3 + 1];
  float z = vertices_[index * 3 + 2];

  float u;
  float v;

  switch (uvMode)
  {
    case UV_XY:
      u = x;//(x - bounds_.minX) / (bounds_.maxX - bounds_.minX);
      v = y;//(y - bounds_.minY) / (bounds_.maxY - bounds_.minY);
      break;
    
    case UV_YZ:
      v = y;//(y - bounds_.minY) / (bounds_.maxY - bounds_.minY);
      u = z;//(z - bounds_.minZ) / (bounds_.maxZ - bounds_.minZ);
      break;

    case UV_ZX:
      v = z;//(z - bounds_.minZ) / (bounds_.maxZ - bounds_.minZ);
      u = x;//(x - bounds_.minX) / (bounds_.maxX - bounds_.minX);
      break;
  }

  vertexData_.push_back(x);
  vertexData_.push_back(y);
  vertexData_.push_back(z);

  vertexData_.push_back(gray); // r
  vertexData_.push_back(gray); // g
  vertexData_.push_back(gray); // b
  
  vertexData_.push_back(u);
  vertexData_.push_back(v);
}

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

void  Object::setUVMode(UVMode mode)
{
  uvMode_ = mode;

}

void  Object::draw() const
{
  glBindVertexArray(VAO_);

  glDrawArrays(GL_TRIANGLES,
               0,
               static_cast<GLsizei>(vertexData_.size() / 8));

  glBindVertexArray(0);
}
