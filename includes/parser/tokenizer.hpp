#pragma once

#include <stdbool.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Tokenizer {
public:
  Tokenizer(const std::string& fileName);
  ~Tokenizer();
  void  tokenize(std::ifstream& file);
  const std::vector<float>            getVertices();
  const std::vector<std::vector<unsigned int>> getFaces();

  private:
  std::string                     fileName_;
  std::vector<float>              vertices_;
  std::vector<std::vector<unsigned int>> faces_; 

};
