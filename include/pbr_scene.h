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
  void ConvertEquirectangularTextureToCubeMap(unsigned int FBO);
  void DrawIrradianceMap(unsigned int captureFBO, unsigned int captureRBO);

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

  // Captures a vertical 90 deg FoV necessary for converting equirectangular
  glm::mat4 capture_projection_ =
      glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
  // One for each side of the cube map
  glm::mat4 capture_views_[6] = {glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
                                             glm::vec3(1.0f, 0.0f, 0.0f),
                                             glm::vec3(0.0f, -1.0f, 0.0f)),
                                 glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
                                             glm::vec3(-1.0f, 0.0f, 0.0f),
                                             glm::vec3(0.0f, -1.0f, 0.0f)),
                                 glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
                                             glm::vec3(0.0f, 1.0f, 0.0f),
                                             glm::vec3(0.0f, 0.0f, 1.0f)),
                                 glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
                                             glm::vec3(0.0f, -1.0f, 0.0f),
                                             glm::vec3(0.0f, 0.0f, -1.0f)),
                                 glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
                                             glm::vec3(0.0f, 0.0f, 1.0f),
                                             glm::vec3(0.0f, -1.0f, 0.0f)),
                                 glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
                                             glm::vec3(0.0f, 0.0f, -1.0f),
                                             glm::vec3(0.0f, -1.0f, 0.0f))};
};

#endif