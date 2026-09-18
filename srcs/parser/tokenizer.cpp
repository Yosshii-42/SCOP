#include "parser/tokenizer.hpp"

Tokenizer::Tokenizer(const std::string& fileName)
  : fileName_(fileName), isFirstVertex_(true) {
    std::ifstream file(fileName.c_str());
    if (!file) {
        throw(std::runtime_error("Failed to open file: " + fileName));
    }
    tokenize(file);
    makeCenter();
    setUVDatas();
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
      setVertices(iss);
    } else if (type == "usemtl") {
      continue;
      // TODO
    } else if (type == "s") {
      continue;
      // TODO
    } else if (type == "f") {   // 
      setFaces(iss);
    }
  }
}

void  Tokenizer::setVertices(std::istringstream& iss)
{
  float x;
  float y;
  float z;
  if (!(iss >> x >> y >> z))
    std::exit(EXIT_FAILURE);
  vertices_.push_back(x);
  vertices_.push_back(y);
  vertices_.push_back(z);

  if (isFirstVertex_)
  {
    minX_ = maxX_ = x;
    minY_ = maxY_ = y;
    minZ_ = maxZ_ = z;
    isFirstVertex_ = false;
  }
  else
  {
    minX_ = std::min(minX_, x);
    maxX_ = std::max(maxX_, x);
    minY_ = std::min(minY_, y);
    maxY_ = std::max(maxY_, y);
    minZ_ = std::min(minZ_, z);
    maxZ_ = std::max(maxZ_, z);
  }
}

void  Tokenizer::setUVDatas()
{
  for (std::vector<float>::iterator it = vertices_.begin();
                                    it != vertices_.end();
                                    it += 3)
  {
    float x = *it;
    float y = *(it + 1);
    float z = *(it + 2);
    float u = (y - minY_) / (maxY_ - minY_);
    float v = (z - minZ_) / (maxZ_ - minZ_);
    vertexDatas_.push_back(x);
    vertexDatas_.push_back(y);
    vertexDatas_.push_back(z);
    vertexDatas_.push_back(u);
    vertexDatas_.push_back(v);
  }
}

void  Tokenizer::setFaces(std::istringstream& iss)
{
  std::vector<unsigned int>  face;
  unsigned int  x;
  while (iss >> x)
    face.push_back(x);
  if (face.size() < 3 || !iss.eof()) 
    std::exit(EXIT_FAILURE);
  faces_.push_back(face);
}

const std::vector<float>  Tokenizer::getVertices()
{
  return (this->vertices_);
}

const std::vector<float>  Tokenizer::getVertexUVs()
{
  return (this->vertexDatas_);
}

const std::vector<std::vector<unsigned int>>  Tokenizer::getFaces()
{
  return (this->faces_);
}

Vec3  Tokenizer::getCenter()
{
  return (center_);
}

void  Tokenizer::makeCenter()
{
  center_.x = (minX_ + maxX_) / 2.0;
  center_.y = (minY_ + maxY_) / 2.0;
  center_.z = (minZ_ + maxZ_) / 2.0;
}
