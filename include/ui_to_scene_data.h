#ifndef UI_TO_SCENE_DATA_H
#define UI_TO_SCENE_DATA_H

#include <string>

namespace pbr_utils {

// Sphere materials
enum Material {
  RUSTED_IRON = 0,
  GOLD = 1,
  GRASS = 2,
  PLASTIC = 3,
  BRICK = 4,
  OAK_WOOD_BARE = 5,
  RECTANGLE_POLISHED_TILE = 6,
  POWDER_COATED_METAL = 7,
  TITANIUM_SCUFFED = 8
};
const int max_materials = 9;

std::string ConvertMaterialToString(const Material& material);
std::string ConvertMaterialToFilePath(const Material& material_path);

extern Material materials[max_materials];
extern std::string material_names[max_materials];

// HDR environments aka Skybox
enum Environment {
  BLUE_NEBULA = 0,
  CITRUS_ORCHARD_PURESKY = 1,
  QUANTANI_NIGHT_PURESKY = 2,
  NIGHT_PORT = 3,
  SUNNY_FIELD = 4,
  SUNNY_SNOW = 5,
  NEWPORT_LOFT = 6,
  EARTHLIKE_PLANET = 7,
};
const int max_environments = 8;

extern Environment environments[max_environments];
extern std::string environment_names[max_environments];

std::string ConvertEnvironmentToString(const Environment& env);
std::string ConvertEnvironmentToFilePath(const Environment& env_path);

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

const float intensity = 200.f;
const float max_intensity = 1000.f;
}  // namespace light_sphere

namespace bloom {
const bool enabled = true;

}

}  // namespace ui_defaults

}  // namespace pbr_utils

#endif