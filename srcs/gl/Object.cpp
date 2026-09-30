#include "gl/Object.hpp"

Object::Object(const std::vector<float>& vertices,
               const std::vector<std::vector<unsigned int> >& faces)
              : vertices_(vertices), faces_(faces), VAO_(0), VBO_(0), EBO_(0)
{
  setIndices();
}

Object::~Object()
{
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
  glDeleteBuffers(1, &EBO_);
}

void  Object::setIndices()
{
  for (size_t i = 0; i < faces_.size(); ++i)
  {
    const std::vector<unsigned int>& face = faces_[i];
    if (face.size() < 3)
      throw std::runtime_error("Face size error");
    for (size_t j = 1; j + 1 < face.size(); ++j)
    {
      indices_.push_back(face[0] - 1);
      indices_.push_back(face[j] - 1);
      indices_.push_back(face[j + 1] - 1);
    }
  }
}

void  Object::setupGPU()
{
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);

  // bind the vertex array object first, then bind and adt vertex buffers, and then cofigure certex attributess.
  glBindVertexArray(VAO_);

  // 頂点データ
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER,
               vertices_.size() * sizeof(float),
               vertices_.data(),
               GL_STATIC_DRAW);
  // glBufferData(GL_ARRAY_BUFFER, vertexUVs.size() * sizeof(float), vertexUVs.data(), GL_STATIC_DRAW);

  // インデックスデータ
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices_.size() * sizeof(unsigned int),
               indices_.data(),
               GL_STATIC_DRAW);

  // 頂点データをどのように解釈すべきかを指示
  // location = 0 = position
  glVertexAttribPointer(0,                  // location = 0
                        3,                  // x, y, z の3要素
                        GL_FLOAT,
                        GL_FALSE,
                        3 * sizeof(float),  // 次の頂点まで3 float
                        (void*)0);          // 先頭から読む
  glEnableVertexAttribArray(0);

  // location = 1 = color
  // glVertexAttribPointer(1,
  //                          3,
  //                          GL_FLOAT,
  //                          GL_FALSE,
  //                          8 * sizeof(float),
  //                         (void*)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);

  // location = 2 = uv座標
  // glVertexAttribPointer(2,
  //                       2,
  //                       GL_FLOAT,
  //                       GL_FALSE,
  //                       5 * sizeof(float),
  //                       (void*)(3 * sizeof(float)));
  // glEnableVertexAttribArray(2);

  // unbind VBO_
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // unbind VAO_
  glBindVertexArray(0);
}

void  Object::draw() const
{
  glBindVertexArray(VAO_);

  glDrawElements(GL_TRIANGLES,
                 static_cast<GLsizei>(indices_.size()),
                 GL_UNSIGNED_INT,
                 0);

  glBindVertexArray(0);
}
