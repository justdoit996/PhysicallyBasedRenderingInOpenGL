#ifndef UTILS_UTILITY_H
#define UTILS_UTILITY_H

#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

bool ReadFile(std::string file,
              std::string& fileContents,
              bool addLineTerminator = false);
long GetFileModTime(std::string file);

unsigned int LoadTexture(const std::string& path);
unsigned int LoadHdrTexture(const std::string& path);

#endif