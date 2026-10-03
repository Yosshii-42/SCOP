#pragma once

#include "math/Vec3.hpp"
#include "Common.hpp"

#include <stdbool.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

class Tokenizer {
private:
  std::vector<float>                      vertices_;
  std::vector<std::vector<unsigned int>>  faces_;
  Common::Bounds                          bounds_;
  Vec3                                    center_;
  bool                                    isFirstVertex_;
  
  void  makeCenter();

public:
  Tokenizer(const std::string& fileName);
  ~Tokenizer();

  void  tokenize(std::ifstream& file);
  void  setVertices(std::istringstream& iss);
  void  setFaces(std::istringstream& iss);

  const std::vector<float>&                     getVertices() const;
  const std::vector<std::vector<unsigned int>>& getFaces() const;
  const Common::Bounds&                         getBounds() const;
  Vec3                                          getCenter() const;
};
