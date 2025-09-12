#include "ui_to_scene_data.h"

namespace pbr_utils {

Material materials[] = {Material::RUSTED_IRON,
                        Material::GOLD,
                        Material::GRASS,
                        Material::PLASTIC,
                        Material::BRICK,
                        Material::OAK_WOOD_BARE,
                        Material::RECTANGLE_POLISHED_TILE,
                        Material::RED_SCIFI_METAL,
                        Material::TITANIUM_SCUFFED};

std::string material_names[] = {
    ConvertMaterialToString(materials[0]),
    ConvertMaterialToString(materials[1]),
    ConvertMaterialToString(materials[2]),
    ConvertMaterialToString(materials[3]),
    ConvertMaterialToString(materials[4]),
    ConvertMaterialToString(materials[5]),
    ConvertMaterialToString(materials[6]),
    ConvertMaterialToString(materials[7]),
    ConvertMaterialToString(materials[8]),
};

std::string ConvertMaterialToString(const Material& material) {
  switch (material) {
    case Material::RUSTED_IRON:
      return "rusted_iron";
    case Material::GOLD:
      return "gold";
    case Material::GRASS:
      return "grass";
    case Material::PLASTIC:
      return "plastic";
    case Material::BRICK:
      return "brick";
    case Material::OAK_WOOD_BARE:
      return "oak-wood-bare";
    case Material::RECTANGLE_POLISHED_TILE:
      return "rectangle-polished-tile";
    case Material::RED_SCIFI_METAL:
      return "red-scifi-metal";
    case Material::TITANIUM_SCUFFED:
      return "titanium-scuffed";
    default:
      return "ERROR: MATERIAL NOT FOUND";
  }
}

std::string ConvertMaterialToFilePath(const Material& material) {
  std::string path_prefix = "resources/assets/textures/pbr/";
  return path_prefix + material_names[static_cast<int>(material)];
}

Environment environments[] = {
    Environment::BLUE_NEBULA,
    Environment::CITRUS_ORCHARD_PURESKY,
    Environment::QUANTANI_NIGHT_PURESKY,
    Environment::NIGHT_PORT,
    Environment::SUNNY_FIELD,
    Environment::SUNNY_SNOW,
    Environment::NEWPORT_LOFT,
};
std::string environment_names[] = {
    ConvertEnvironmentToString(Environment::BLUE_NEBULA),
    ConvertEnvironmentToString(Environment::CITRUS_ORCHARD_PURESKY),
    ConvertEnvironmentToString(Environment::QUANTANI_NIGHT_PURESKY),
    ConvertEnvironmentToString(Environment::NIGHT_PORT),
    ConvertEnvironmentToString(Environment::SUNNY_FIELD),
    ConvertEnvironmentToString(Environment::SUNNY_SNOW),
    ConvertEnvironmentToString(Environment::NEWPORT_LOFT),
};

std::string ConvertEnvironmentToString(const Environment& env) {
  switch (env) {
    case Environment::NEWPORT_LOFT:
      return "newport_loft";
    case Environment::CITRUS_ORCHARD_PURESKY:
      return "citrus_orchard_puresky";
    case Environment::QUANTANI_NIGHT_PURESKY:
      return "qwantani_night_puresky";
    case Environment::NIGHT_PORT:
      return "night_port";
    case Environment::SUNNY_FIELD:
      return "sunny_field";
    case Environment::SUNNY_SNOW:
      return "sunny_snow";
    case Environment::BLUE_NEBULA:
      return "blue_nebula";
    default:
      return "ERORR: ENVIRONMENT NOT FOUND";
  }
}

std::string ConvertEnvironmentToFilePath(const Environment& env) {
  std::string path_prefix = "resources/assets/textures/hdr/";
  return path_prefix + environment_names[static_cast<int>(env)] + ".hdr";
}
};  // namespace pbr_utils