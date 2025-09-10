#ifndef UI_TO_SCENE_DATA_H
#define UI_TO_SCENE_DATA_H

#include <string>

namespace pbr_scene {

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
std::string ConvertMaterialToFilePath(const Material& material);

extern pbr_scene::Material materials[9];
extern std::string material_names[9];

enum Environment {
  SKY_LOFT = 0,
};

}  // namespace pbr_scene
#endif