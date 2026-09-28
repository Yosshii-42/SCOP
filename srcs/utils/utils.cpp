#include "utils/utils.hpp"

void  Utils::checkArgv(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Argument number error" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  struct stat status;

  if (stat(argv[1], &status) != 0) {
    std::cerr << "Failed to stat file: " << argv[1] << std::endl;
    std::exit(EXIT_FAILURE);
  }
  if (status.st_mode & S_IFDIR) {
    std::cerr << argv[1] << ": is a directory" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::string fileName(argv[1]);
  if (fileName.size() < 4 ||
      fileName.compare(fileName.size() - 4, 4, ".obj") != 0) {
      std::cerr << "filename must have \".obj\" extention" << std::endl;
      std::exit(EXIT_FAILURE);
  }
}
