#ifndef PBR_SCENE_H
#define PBR_SCENE_H

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include <glm/glm.hpp>
#include <memory>
#include "camera.h"
#include "point_light.h"
#include "shaders/brdf_shader.h"
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
  void UploadPbrTextures(const std::string& path);
  void UploadHdrMap(const std::string& path);
  // Called AFTER Init().
  // Can be used after loading new environment (hdr) maps
  // Performs all pre-computations/convolutions, look-up tables, and
  // framebuffers necessary before rendering loop.
  void InitAllTextureMaps();

 private:
  void Init();
  void ConvertEquirectangularTextureToCubeMap();
  // Diffuse part of PBR
  // Pre convolve cube map to construct an irradiance cubemap texture.
  void DrawIrradianceMap();
  // Specular part of PBR (1/2)
  // Creates a cube map for the first part of the split-sum approximation
  void DrawPreFilteredEnvironmentMap();
  // Specular part of PBR (2/2)
  // Creates a LUT for the brdf part of the split-sum approximation
  void DrawBrdfIntegrationMap();
  void CreateAndBindFramebufferAndRenderBufferObjects();

  // Framebuffer and renderbuffer objects
  unsigned int capture_fbo_;
  unsigned int capture_rbo_;

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
  BrdfShader brdf_shader_;

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