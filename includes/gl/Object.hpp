#pragma once

#include <glad/glad.h>

#include <vector>

class Object {
private:
  std::vector<float>                      vertices_;
  std::vector<std::vector<unsigned int>>  faces_;
  std::vector<unsigned int>               indices_;

  unsigned int  VAO_;
  unsigned int  VBO_;
  unsigned int  EBO_;
  
  Object(const Object&) = delete;
  Object& operator=(const Object&) = delete;

public:
  Object(const std::vector<float>& vertices,
          const std::vector<std::vector<unsigned int> >& faces);
  ~Object();

  void  setIndices(); 
  void  setupGPU();
  void  draw() const;
};

// setupGPU()の担当範囲
// VAO作成
//  ↓
// VBO作成
//  ↓
// EBO作成
//  ↓
// vertices_ → VBO
//  ↓
// indices_ → EBO
//  ↓
// 頂点属性の配線
//  ↓
// VAOに設定を記憶させる