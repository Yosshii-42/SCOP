#include "parser/tokenizer.hpp"

Tokenizer::Tokenizer(const std::string& fileName)
  : fileName_(fileName) {
    std::ifstream file(fileName.c_str());
    if (!file) {
        throw(std::runtime_error("Failed to open file: " + fileName));
    }
    tokenize(file);
}

Tokenizer::~Tokenizer() {}

void  Tokenizer::tokenize(std::ifstream& file) {
  std::string line;
  while (std::getline(file, line)) {
    std::istringstream  iss(line);
    std::string         type;

    iss >> type;
    if (type.empty())
      continue;
    if (type[0] == '#')
      continue;
    if (type == "mtllib") {
      continue;
      // TODO
    } else if (type == "o") {
      continue;
      // TODO
    } else if (type == "v") {   // 頂点座標を収納
      float x;
      float y;
      float z;
      if (!(iss >> x >> y >> z))
        std::exit(EXIT_FAILURE);
      vertices_.push_back(x);
      vertices_.push_back(y);
      vertices_.push_back(z);
    } else if (type == "usemtl") {
      continue;
      // TODO
    } else if (type == "s") {
      continue;
      // TODO
    } else if (type == "f") {   // 
      std::vector<unsigned int>  face;
      unsigned int  x;
      while (iss >> x)
        face.push_back(x);
      if (face.size() < 3 || !iss.eof()) 
        std::exit(EXIT_FAILURE);
      faces_.push_back(face);
    }
  }
}

const std::vector<float>  Tokenizer::getVertices() {
  return (this->vertices_);
}

const std::vector<std::vector<unsigned int>>  Tokenizer::getFaces() {
  return (this->faces_);
}
