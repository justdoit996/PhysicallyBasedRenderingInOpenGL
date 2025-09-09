#ifndef PBR_SCENE_H
#define PBR_SCENE_H

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include <glm/glm.hpp>
#include <memory>
#include "camera.h"
#include "point_light.h"
#include "shaders/environment_cube_map_shader.h"
#include "shaders/equirectangular_to_cube_map_shader.h"
#include "shaders/irradiance_cube_map_shader.h"
#include "shaders/prefilter_shader.h"
#include "shaders/shader.h"
#include "shaders/sphere_shader.h"
#include "shapes/cube.h"
#include "shapes/quad.h"
#include "shapes/sphere.h"
#include "utils/constants.h"

class PbrScene {
 public:
  PbrScene() = default;
  PbrScene(Camera* camera);
  void Render();

 private:
  void Init();
  void DrawCubeMapToFramebuffer();

  // Shapes for generating, binding, or drawing vertices
  std::unique_ptr<Sphere> sphere_;
  std::unique_ptr<Cube> cube_map_cube_;
  std::unique_ptr<Quad> quad_;

  // Camera
  Camera* camera_ = nullptr;

  // Shaders
  SphereShader sphere_shader_;
  EquirectangularToCubeMapShader equirectangular_to_cube_map_shader_;
  EnvironmentCubeMapShader environment_cube_map_shader_;
  IrradianceCubeMapShader irradiance_cube_map_shader_;
  PrefilterShader prefilter_shader_;

  // Lights
  std::vector<PointLight> point_lights_;
};

#endif