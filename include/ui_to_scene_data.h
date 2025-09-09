#ifndef UI_TO_SCENE_DATA_H
#define UI_TO_SCENE_DATA_H

struct UiToSceneData {
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

  UiToSceneData::Material material = UiToSceneData::Material::RUSTED_IRON;
  UiToSceneData::Environment environment = UiToSceneData::Environment::SKY_LOFT;
};

#endif