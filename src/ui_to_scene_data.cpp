#include "ui_to_scene_data.h"

namespace pbr_scene {
std::string ConvertToFilePath(const Material& material) {
  switch (material) {
    case Material::RUSTED_IRON:
      return "resources/assets/textures/pbr/rusted_iron";
      break;
    case Material::GOLD:
      return "resources/assets/textures/pbr/gold";
      break;
    case Material::GRASS:
      return "resources/assets/textures/pbr/grass";
      break;
    case Material::PLASTIC:
      return "resources/assets/textures/pbr/plastic";
      break;
    case Material::BRICK:
      return "resources/assets/textures/pbr/brick";
      break;
    default:
      return "ERROR: MATERIAL NOT FOUND";
      break;
  }
}
};  // namespace pbr_scene