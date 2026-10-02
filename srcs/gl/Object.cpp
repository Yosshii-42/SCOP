#include "gl/Object.hpp"

Object::Object(const std::vector<float>& vertices,
               const std::vector<std::vector<unsigned int> >& faces)
              : vertices_(vertices), faces_(faces), VAO_(0), VBO_(0)
{
  setVertexData();
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
    // float gray = 0.5f + (i % 4) * 0.15f; // 規則的な模様が出る
    float gray = 0.3f + static_cast<float>(std::rand() % 46) / 100.0f; // ランダムな模様が出る
    for (size_t j = 1; j + 1 < face.size(); ++j)
    {
      // 3つの頂点のface番号を取得する
      unsigned int  index1 = face[0] - 1;
      unsigned int  index2 = face[j] - 1;
      unsigned int  index3 = face[j + 1] - 1;

      // face番号を元に、3つの頂点の座興データを取得し、vertexData_にpush_backする
      addVertexData(index1, gray);
      addVertexData(index2, gray);
      addVertexData(index3, gray);
    }
  }
}

void  Object::addVertexData(unsigned int index, float gray)
{
  vertexData_.push_back(vertices_[index * 3]);      // x
  vertexData_.push_back(vertices_[index * 3 + 1]);  // y
  vertexData_.push_back(vertices_[index * 3 + 2]);  // z
  vertexData_.push_back(gray);                      // r
  vertexData_.push_back(gray);                      // g
  vertexData_.push_back(gray);                      // b
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
                        6 * sizeof(float),  // 次の頂点まで6 float
                        (void*)0);          // 先頭から読む
  glEnableVertexAttribArray(0);

  // location = 1 = color
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        6 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

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

  glDrawArrays(GL_TRIANGLES,
               0,
               static_cast<GLsizei>(vertexData_.size() / 6));

  glBindVertexArray(0);
}
