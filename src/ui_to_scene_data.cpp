#include "ui_to_scene_data.h"

namespace pbr_scene {

pbr_scene::Material materials[] = {
    pbr_scene::Material::RUSTED_IRON, pbr_scene::Material::GOLD,
    pbr_scene::Material::GRASS,       pbr_scene::Material::PLASTIC,
    pbr_scene::Material::BRICK,
};
std::string material_names[] = {
    pbr_scene::ConvertMaterialToString(materials[0]),
    pbr_scene::ConvertMaterialToString(materials[1]),
    pbr_scene::ConvertMaterialToString(materials[2]),
    pbr_scene::ConvertMaterialToString(materials[3]),
    pbr_scene::ConvertMaterialToString(materials[4]),
};

std::string ConvertMaterialToString(const Material& material) {
  switch (material) {
    case Material::RUSTED_IRON:
      return "rusted_iron";
      break;
    case Material::GOLD:
      return "gold";
      break;
    case Material::GRASS:
      return "grass";
      break;
    case Material::PLASTIC:
      return "plastic";
      break;
    case Material::BRICK:
      return "brick";
      break;
    default:
      return "ERROR: MATERIAL NOT FOUND";
      break;
  }
}

std::string ConvertMaterialToFilePath(const Material& material) {
  switch (material) {
    case Material::RUSTED_IRON:
      return "resources/assets/textures/pbr/" +
             ConvertMaterialToString(material);
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