#ifndef IBL_RENDERER_H
#define IBL_RENDERER_H

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "shaders/brdf_shader.h"
#include "shaders/environment_cube_map_shader.h"
#include "shaders/equirectangular_to_cube_map_shader.h"
#include "shaders/irradiance_cube_map_shader.h"
#include "shaders/prefilter_shader.h"
#include "shapes/cube.h"
#include "shapes/quad.h"

class IblRenderer {
 public:
  IblRenderer() = default;
  void InitAllTextureMaps();

 private:
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

  // Shapes
  std::unique_ptr<Cube> cube_map_cube_;
  std::unique_ptr<Quad> quad_;

  // Framebuffer and renderbuffer objects
  unsigned int capture_fbo_;
  unsigned int capture_rbo_;

  // Shaders
  EquirectangularToCubeMapShader equirectangular_to_cube_map_shader_;
  EnvironmentCubeMapShader environment_cube_map_shader_;
  IrradianceCubeMapShader irradiance_cube_map_shader_;
  PrefilterShader prefilter_shader_;
  BrdfShader brdf_shader_;

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