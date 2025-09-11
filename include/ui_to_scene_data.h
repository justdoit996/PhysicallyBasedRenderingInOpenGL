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
const int max_materials = 9;

std::string ConvertMaterialToString(const Material& material);
std::string ConvertMaterialToFilePath(const Material& material_path);

extern Material materials[max_materials];
extern std::string material_names[max_materials];

// HDR environments aka Skybox
enum Environment {
  NEWPORT_LOFT = 0,
  CITRUS_ORCHARD_PURESKY = 1,
  QUANTANI_NIGHT_PURESKY = 2,
  NIGHT_PORT = 3,
  SUNNY_FIELD = 4,
  SUNNY_SNOW = 5,
  BLUE_NEBULA = 6,
};
const int max_environments = 7;

extern Environment environments[max_environments];
extern std::string environment_names[max_environments];

std::string ConvertEnvironmentToString(const Environment& env);
std::string ConvertEnvironmentToFilePath(const Environment& env_path);
}  // namespace pbr_scene

#endif