#ifndef UTILS_UTILITY_H
#define UTILS_UTILITY_H

#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool ReadFile(std::string file,
              std::string& fileContents,
              bool addLineTerminator = false);
long GetFileModTime(std::string file);

unsigned int LoadTexture(const std::string& path);
unsigned int LoadHdrTexture(const std::string& path);

namespace pbr_utils {

std::vector<std::string> getFileNamesInDirectory(const std::string&);

extern std::vector<std::string> material_names;

extern std::vector<std::string> environment_names;

}  // namespace pbr_utils

#endif