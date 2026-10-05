#include "gl/Texture.hpp"
#include "parser/BMP.hpp"

Texture::Texture(const std::string& path)
  : id_(0)
{
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);

  // set the texture wrapping/filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //loat and geterate the texture
  BMP bmp("img/wall.bmp");
  const std::vector<unsigned char>& data = bmp.getData();
  if (data.empty())
  {
    glDeleteTextures(1, &id_);
    id_ = 0;
    throw std::runtime_error("Failed to load Texture: " + path);
  }
  
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGB,
               bmp.getWidth(),
               bmp.getHeight(),
               0,
               GL_RGB,
               GL_UNSIGNED_BYTE,
               data.data());
  glGenerateMipmap(GL_TEXTURE_2D); 

  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
  glDeleteTextures(1, &id_);
}

void  Texture::bind() const
{
  glBindTexture(GL_TEXTURE_2D, id_);
}
