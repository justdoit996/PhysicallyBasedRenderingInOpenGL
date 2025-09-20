#ifndef PBR_SCENE_H
#define PBR_SCENE_H

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include <glm/glm.hpp>
#include <memory>
#include "bloom_renderer.h"
#include "camera.h"
#include "ibl_renderer.h"
#include "point_light.h"
#include "shaders/framebuffer_shader.h"
#include "shaders/shader.h"
#include "shaders/sphere_shader.h"
#include "shapes/quad.h"
#include "shapes/sphere.h"
#include "ui_to_scene_data.h"
#include "utils/constants.h"

class PbrScene {
 public:
  PbrScene() = default;
  PbrScene(Camera* camera);

  void Render();
  void UploadPbrTextures(const std::string& path);
  void UploadHdrMap(const std::string& path);
  void InitAllIblTextureMaps();

  void SetPointLightEnabled(bool enable);
  void SetRedColor(float r);
  void SetGreenColor(float g);
  void SetBlueColor(float b);
  void SetLightIntensity(float intense);
  void SetBloomEnabled(bool bloom);
  void SetBloomStrength(float str);

 private:
  void Init();

  // Shapes for generating, binding, or drawing vertices
  std::unique_ptr<Sphere> sphere_;
  std::unique_ptr<Quad> quad_;

  // Camera
  Camera* camera_ = nullptr;

  // IBL pre-rendering
  IblRenderer ibl_renderer_;

  // Shaders
  SphereShader sphere_shader_;
  Shader light_sphere_shader_;
  FramebufferShader framebuffer_to_screen_shader_;

  // Bloom
  BloomRenderer bloom_renderer_;
  bool bloom_enabled_ = constants::ui_defaults::bloom::enabled;
  float bloom_strength_ = constants::ui_defaults::bloom::strength;

  // Point Light Sphere
  PointLight point_light_;
  bool point_light_enabled_ = constants::ui_defaults::light_sphere::enabled;
};

#endif