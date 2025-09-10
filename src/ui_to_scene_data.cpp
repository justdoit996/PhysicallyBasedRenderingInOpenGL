#include "ui_to_scene_data.h"

namespace pbr_scene {

pbr_scene::Material materials[] = {pbr_scene::Material::RUSTED_IRON,
                                   pbr_scene::Material::GOLD,
                                   pbr_scene::Material::GRASS,
                                   pbr_scene::Material::PLASTIC,
                                   pbr_scene::Material::BRICK,
                                   pbr_scene::Material::OAK_WOOD_BARE,
                                   pbr_scene::Material::RECTANGLE_POLISHED_TILE,
                                   pbr_scene::Material::RED_SCIFI_METAL,
                                   pbr_scene::Material::TITANIUM_SCUFFED};

std::string material_names[] = {
    pbr_scene::ConvertMaterialToString(materials[0]),
    pbr_scene::ConvertMaterialToString(materials[1]),
    pbr_scene::ConvertMaterialToString(materials[2]),
    pbr_scene::ConvertMaterialToString(materials[3]),
    pbr_scene::ConvertMaterialToString(materials[4]),
    pbr_scene::ConvertMaterialToString(materials[5]),
    pbr_scene::ConvertMaterialToString(materials[6]),
    pbr_scene::ConvertMaterialToString(materials[7]),
    pbr_scene::ConvertMaterialToString(materials[8]),
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
    case Material::OAK_WOOD_BARE:
      return "oak-wood-bare";
      break;
    case Material::RECTANGLE_POLISHED_TILE:
      return "rectangle-polished-tile";
      break;
    case Material::RED_SCIFI_METAL:
      return "red-scifi-metal";
      break;
    case Material::TITANIUM_SCUFFED:
      return "titanium-scuffed";
      break;
    default:
      return "ERROR: MATERIAL NOT FOUND";
      break;
  }
}

std::string ConvertMaterialToFilePath(const Material& material) {
  std::string path_prefix = "resources/assets/textures/pbr/";
  return path_prefix + material_names[static_cast<int>(material)];
}
};  // namespace pbr_scene