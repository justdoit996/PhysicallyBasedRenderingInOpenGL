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

namespace ui_defaults {

namespace light_sphere {
const bool enabled = true;
const int red = 255;
const int green = 255;
const int blue = 255;
const int rgb_max = 255;

const float redf = 1.f * red / rgb_max;
const float greenf = 1.f * green / rgb_max;
const float bluef = 1.f * blue / rgb_max;

const float intensity = 100.f;
const float max_intensity = 200.f;
}  // namespace light_sphere

namespace bloom {
const bool enabled = true;
const float strength = 0.004;
const float max_strength = 0.1;
}  // namespace bloom

}  // namespace ui_defaults

}  // namespace pbr_utils

#endif