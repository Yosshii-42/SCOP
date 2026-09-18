#pragma once

#include "math/Vec3.hpp"

#include <stdbool.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

class Tokenizer {
public:

Tokenizer(const std::string& fileName);
~Tokenizer();
void  tokenize(std::ifstream& file);
void  setVertices(std::istringstream& iss);
void  setUVDatas();
void  setFaces(std::istringstream& iss);
const std::vector<float>                      getVertices();
const std::vector<float>                      getVertexUVs();
const std::vector<std::vector<unsigned int>>  getFaces();
Vec3  getCenter();

private:
  std::string                     fileName_;
  std::vector<float>              vertices_;
  std::vector<float>              vertexDatas_;
  std::vector<std::vector<unsigned int>> faces_;
  void  makeCenter();

  float minX_;
  float maxX_;
  float minY_;
  float maxY_;
  float minZ_;
  float maxZ_;
  bool  isFirstVertex_;
  Vec3  center_;
};
