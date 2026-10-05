#include "parser/Tokenizer.hpp"

Tokenizer::Tokenizer(const std::string& fileName)
  : isFirstVertex_(true)
{
    std::ifstream file(fileName.c_str());
    if (!file) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }
    tokenize(file);
    makeCenter();
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

  // min, maxを更新する
  if (isFirstVertex_)
  {
    bounds_.minX = bounds_.maxX = x;
    bounds_.minY = bounds_.maxY = y;
    bounds_.minZ = bounds_.maxZ = z;
    isFirstVertex_ = false;
  }
  else
  {
    bounds_.minX = std::min(bounds_.minX, x);
    bounds_.maxX = std::max(bounds_.maxX, x);
    bounds_.minY = std::min(bounds_.minY, y);
    bounds_.maxY = std::max(bounds_.maxY, y);
    bounds_.minZ = std::min(bounds_.minZ, z);
    bounds_.maxZ = std::max(bounds_.maxZ, z);
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

const std::vector<float>&  Tokenizer::getVertices() const
{
  return (this->vertices_);
}

const std::vector<std::vector<unsigned int>>&  Tokenizer::getFaces() const
{
  return (this->faces_);
}

const Common::Bounds&  Tokenizer::getBounds() const
{
  return (bounds_);
}

Vec3  Tokenizer::getCenter() const
{
  return (center_);
}

void  Tokenizer::makeCenter()
{
  center_.x = (bounds_.minX + bounds_.maxX) / 2.0;
  center_.y = (bounds_.minY + bounds_.maxY) / 2.0;
  center_.z = (bounds_.minZ + bounds_.maxZ) / 2.0;
}
