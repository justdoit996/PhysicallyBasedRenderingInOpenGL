#ifndef UI_TO_SCENE_DATA_H
#define UI_TO_SCENE_DATA_H

#include "pbr_scene.h"

struct UiToSceneData {
  PbrScene::Material material = PbrScene::Material::RUSTED_IRON;
  PbrScene::Environment environment = PbrScene::Environment::SKY_LOFT;
};

#endif