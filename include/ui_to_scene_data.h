#ifndef UI_TO_SCENE_DATA_H
#define UI_TO_SCENE_DATA_H

#include <string>

namespace pbr_scene {

// Sphere materials
enum Material {
  RUSTED_IRON = 0,
  GOLD = 1,
  GRASS = 2,
  PLASTIC = 3,
  BRICK = 4,
  OAK_WOOD_BARE = 5,
  RECTANGLE_POLISHED_TILE = 6,
  RED_SCIFI_METAL = 7,
  TITANIUM_SCUFFED = 8
};

std::string ConvertMaterialToString(const Material& material);
std::string ConvertMaterialToFilePath(const Material& material_path);

extern Material materials[9];
extern std::string material_names[9];

// HDR environments aka Skybox
enum Environment {
  NEWPORT_LOFT = 0,
  CITRUS_ORCHARD_PURESKY = 1,
  QUANTANI_NIGHT_PURESKY = 2
};

extern Environment environments[3];
extern std::string environment_names[3];

std::string ConvertEnvironmentToString(const Environment& env);
std::string ConvertEnvironmentToFilePath(const Environment& env_path);
}  // namespace pbr_scene

#endif