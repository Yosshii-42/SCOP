#include "parser/BMP.hpp"

BMP::BMP(const std::string& path)
  : width_(0), height_(0)
{
  std::ifstream file(path.c_str(), std::ios::binary);
  if (!file)
    throw std::runtime_error("Failed to open BMP: " + path);

  // 先頭の2文字を判定する
  char  signature[2];
  file.read(signature, 2); // fileには入出力に関する情報が保存される
  if (!file)
    throw std::runtime_error("Failed to read BMP header");
  if (signature[0] != 'B' || signature[1] != 'M')
    throw std::runtime_error("Invalid BMP file");

  // pixel offsetを取得する
  unsigned int  pixelOffset;
  file.seekg(10, std::ios::beg); // 最初から10進める
  // pixelOffsetを指すunsigned int*を、read()に渡すためchar*として扱う
  file.read(reinterpret_cast<char*>(&pixelOffset),
            sizeof(pixelOffset));
  if (!file)
    throw std::runtime_error("Failed to read BMP pixel offset");

  // width, heightを取得する
  file.seekg(18, std::ios::beg);
  file.read(reinterpret_cast<char*>(&width_),
            sizeof(width_));
  if (!file)
    throw std::runtime_error("Failed to read BMP width");  
  file.seekg(22, std::ios::beg);
  file.read(reinterpret_cast<char*>(&height_),
            sizeof(height_));
  if (!file)
    throw std::runtime_error("Failed to read BMP height");

  // bitsPerPixel, compression読み込み
  unsigned short  bitsPerPixel;
  unsigned int    compression;
  file.seekg(28, std::ios::beg);
  file.read(reinterpret_cast<char*>(&bitsPerPixel),
            sizeof(bitsPerPixel));
  file.read(reinterpret_cast<char*>(&compression),
            sizeof(compression));
  if (!file)
    throw std::runtime_error("Failed to read BMP bits per pixel and compresstion");
  if (bitsPerPixel != 24)
    throw std::runtime_error("Only 24-bit BMP is supported");
  if (compression != 0)
    throw std::runtime_error("Compressed BMP is not supported");
  
  size_t  rowSize = (width_ * 3 + 3) / 4 * 4; // 4の倍数まで切り上げる
  size_t  padding = rowSize - width_ * 3;

  file.seekg(pixelOffset, std::ios::beg);
  for (int y = 0; y < height_; ++y)
  {
    for (int x = 0; x < width_; ++x)
    {
      // 正の整数の読み込み
      unsigned char b;
      unsigned char g;
      unsigned char r;

      file.read(reinterpret_cast<char*>(&b), sizeof(char));
      file.read(reinterpret_cast<char*>(&g), sizeof(char));
      file.read(reinterpret_cast<char*>(&r), sizeof(char));

      data_.push_back(r);
      data_.push_back(g);
      data_.push_back(b);

      // if (y == height_ / 2
      //     && x >= width_ / 2
      //     && x < width_ / 2 + 10)
      // {
      //   std::cout << "R=" << static_cast<int>(r)
      //             << " G=" << static_cast<int>(g)
      //             << " B=" << static_cast<int>(b)
      //             << std::endl;
      // }
    }
    file.seekg(padding, std::ios::cur);
  }
}
// file.read(char* buffer, std::streamsize size);

BMP::~BMP() {}

int BMP::getWidth() const
{
  return (width_);
}

int BMP::getHeight() const
{
  return (height_);
}

const std::vector<unsigned char>&  BMP::getData() const
{
  return (data_);
}

//  0 ─  1  "BM"
//  2 ─  5  file size
//  6 ─  9  reserved
// 10 ─ 13  Pixel Dataの開始位置  ← 欲しい
// 14 ─ 17  DIB Header size
// 18 ─ 21  width
// 22 ─ 25  height
// 28 ─ 29  bitsPerPixel   2 bytes
// 30 ─ 33  compression    4 bytes
