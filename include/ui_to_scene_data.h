#ifndef UI_TO_SCENE_DATA_H
#define UI_TO_SCENE_DATA_H

#include <string>

#include <filesystem>
#include <iostream>
#include <vector>

namespace pbr_utils {

std::vector<std::string> getFileNamesInDirectory(const std::string&);

extern std::vector<std::string> material_names;

extern std::vector<std::string> environment_names;

}  // namespace pbr_utils

#endif