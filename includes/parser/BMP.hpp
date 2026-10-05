#pragma once

#include <iostream>
#include <fstream>  // BMPをバイナリで開く
#include <vector>
#include <string>

class BMP {
private:
  int width_;
  int height_;
  std::vector<unsigned char>  data_;

public:
  explicit BMP(const std::string& path);
  ~BMP();
  BMP(const BMP& other) = delete;
  BMP& operator=(const BMP& other) = delete;

  int                   getWidth() const;
  int                   getHeight() const;
  const std::vector<unsigned char>&  getData() const;
};
