#pragma once

#include <glad/glad.h>

#include <vector>

class Object {
private:
  std::vector<float>                      vertices_;
  std::vector<std::vector<unsigned int>>  faces_;
  std::vector<float>                      vertexData_;  // 色情報を入れたvertexデータ

  unsigned int  VAO_;
  unsigned int  VBO_;
  
  void  setVertexData();
  void  addVertexData(unsigned int index, float gray);
  
public:
  Object(const std::vector<float>& vertices,
         const std::vector<std::vector<unsigned int> >& faces);
  ~Object();
  Object(const Object&) = delete;
  Object& operator=(const Object&) = delete;

  void  setupGPU();
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