#pragma once

#include <glad/glad.h>

#include <stdexcept>
#include <string>

class Texture {
private:
  unsigned int  id_;

public:
  explicit Texture(const std::string& path);
  ~Texture();
  Texture(const Texture& other) = delete;
  Texture& operator=(const Texture& other) = delete;

  void  bind() const;
};
