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
};

enum Environment {
  SKY_LOFT = 0,
};

struct UiToSceneData {};

std::string ConvertMaterialToString(const Material& material);
std::string ConvertMaterialToFilePath(const Material& material);

}  // namespace pbr_scene
#endif