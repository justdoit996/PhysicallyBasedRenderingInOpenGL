#ifndef UTILS_UTILITY_H
#define UTILS_UTILITY_H

#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

bool ReadFile(std::string file,
              std::string& fileContents,
              bool addLineTerminator = false);
long GetFileModTime(std::string file);

#endif