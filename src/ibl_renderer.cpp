#include "ibl_renderer.h"

#include "ui_to_scene_data.h"
#include "utils/constants.h"

IblRenderer::IblRenderer() {
  // Shaders
  equirectangular_to_cube_map_shader_ = EquirectangularToCubeMapShader(
      "resources/shaders/pbr/equirectangular.vs",
      "resources/shaders/pbr/equirectangular.fs");
  environment_cube_map_shader_ =
      EnvironmentCubeMapShader("resources/shaders/pbr/env_cube_map.vs",
                               "resources/shaders/pbr/env_cube_map.fs");
  // Reuse equirectangular vertex shader file
  irradiance_cube_map_shader_ =
      IrradianceCubeMapShader("resources/shaders/pbr/equirectangular.vs",
                              "resources/shaders/pbr/irradiance_cube_map.fs");
  // Reuse equirectangular vertex shader file
  prefilter_shader_ =
      PrefilterShader("resources/shaders/pbr/equirectangular.vs",
                      "resources/shaders/pbr/prefilter_map.fs");
  brdf_shader_ = BrdfShader("resources/shaders/pbr/brdf.vs",
                            "resources/shaders/pbr/brdf.fs");

  equirectangular_to_cube_map_shader_.LoadTextures(
      "resources/assets/textures/hdr/" + pbr_utils::environment_names[0]);
  environment_cube_map_shader_.GenerateTextures();
  irradiance_cube_map_shader_.GenerateTextures();
  prefilter_shader_.GenerateTextures();
  brdf_shader_.GenerateTextures();

  cube_map_cube_ = std::make_unique<Cube>();
  quad_ = std::make_unique<Quad>();

  glm::mat4 camera_perspective_projection =
      glm::perspective(constants::camera::ZOOM, constants::ASPECT_RATIO,
                       constants::camera::NEAR, constants::camera::FAR);
  environment_cube_map_shader_.Use();
  environment_cube_map_shader_.SetMat4("projection",
                                       camera_perspective_projection);
}

void IblRenderer::InitAllTextureMaps() {
  CreateAndBindFramebufferAndRenderBufferObjects();
  ConvertEquirectangularTextureToCubeMap();
  DrawIrradianceMap();
  DrawPreFilteredEnvironmentMap();
  DrawBrdfIntegrationMap();

  // Check framebuffer is complete
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Framebuffer not complete!" << std::endl;
    exit(1);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_RENDERBUFFER, 0);
}

void IblRenderer::CreateAndBindFramebufferAndRenderBufferObjects() {
  // Create and bind framebuffer and renderbuffer
  glGenFramebuffers(1, &capture_fbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo_);
  glGenRenderbuffers(1, &capture_rbo_);
  glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo_);
  // Attach renderbuffer to framebuffer
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, capture_rbo_);
}

void IblRenderer::ConvertEquirectangularTextureToCubeMap() {
  // Draw equirectangular map to 6 sided cubemap framebuffer
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                        constants::environment_cubemap_width,
                        constants::environment_cubemap_width);
  equirectangular_to_cube_map_shader_.Use();
  equirectangular_to_cube_map_shader_.SetMat4("projection",
                                              capture_projection_);
  equirectangular_to_cube_map_shader_.BindAllTextures();
  glViewport(0, 0, constants::environment_cubemap_width,
             constants::environment_cubemap_width);
  glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo_);
  for (unsigned int i = 0; i < 6; ++i) {
    equirectangular_to_cube_map_shader_.SetMat4("view", capture_views_[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                           environment_cube_map_shader_.env_cube_map_texture(),
                           0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cube_map_cube_->Draw();
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_CUBE_MAP,
                environment_cube_map_shader_.env_cube_map_texture());
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void IblRenderer::DrawIrradianceMap() {
  // Draw irradiance convolution map to 6 sided cubemap framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo_);
  glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                        constants::irradiance_map_dimension,
                        constants::irradiance_map_dimension);
  irradiance_cube_map_shader_.Use();
  irradiance_cube_map_shader_.SetMat4("projection", capture_projection_);
  environment_cube_map_shader_.BindAllTextures();
  glViewport(0, 0, constants::irradiance_map_dimension,
             constants::irradiance_map_dimension);
  glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo_);
  for (unsigned int i = 0; i < 6; ++i) {
    irradiance_cube_map_shader_.SetMat4("view", capture_views_[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                           irradiance_cube_map_shader_.irradiance_map_texture(),
                           0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cube_map_cube_->Draw();
  }
}

void IblRenderer::DrawPreFilteredEnvironmentMap() {
  // Prefilter HDR map
  prefilter_shader_.Use();
  prefilter_shader_.SetMat4("projection", capture_projection_);
  environment_cube_map_shader_.BindAllTextures();
  glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo_);
  unsigned int max_mip_levels = 5;
  for (unsigned int mip_level = 0; mip_level < max_mip_levels; ++mip_level) {
    // resize framebuffer according to mip-level size.
    unsigned int mipWidth = static_cast<unsigned int>(
        constants::prefilter_map_dimension * std::pow(0.5, mip_level));
    unsigned int mipHeight = static_cast<unsigned int>(
        constants::prefilter_map_dimension * std::pow(0.5, mip_level));
    glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth,
                          mipHeight);
    glViewport(0, 0, mipWidth, mipHeight);

    float roughness = (float)mip_level / (float)(max_mip_levels - 1);
    prefilter_shader_.SetFloat("roughness", roughness);
    for (unsigned int i = 0; i < 6; ++i) {
      prefilter_shader_.SetMat4("view", capture_views_[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             prefilter_shader_.prefilter_map_texture(),
                             mip_level);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      cube_map_cube_->Draw();
    }
  }
}

void IblRenderer::DrawBrdfIntegrationMap() {
  glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo_);
  glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                        constants::brdf_integration_map_dimension,
                        constants::brdf_integration_map_dimension);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         brdf_shader_.brdf_lut_texture(), 0);
  glViewport(0, 0, constants::brdf_integration_map_dimension,
             constants::brdf_integration_map_dimension);
  brdf_shader_.Use();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  quad_->Draw();
}

void IblRenderer::UploadHdrMap(const std::string& path) {
  equirectangular_to_cube_map_shader_.LoadTextures(path);
}

void IblRenderer::RenderEnvironment(const glm::mat4& view) {
  // Render skybox (render background last to prevent overdrawing)
  environment_cube_map_shader_.Use();
  environment_cube_map_shader_.SetMat4("view", view);
  environment_cube_map_shader_.BindAllTextures();
  cube_map_cube_->Draw();
}

// TODO: Fix manual texture mapping. Needs to be syncd with sphere shader's
// textures
void IblRenderer::BindAllTextures() {
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_CUBE_MAP,
                irradiance_cube_map_shader_.irradiance_map_texture());
  glActiveTexture(GL_TEXTURE6);
  glBindTexture(GL_TEXTURE_CUBE_MAP, prefilter_shader_.prefilter_map_texture());
  glActiveTexture(GL_TEXTURE7);
  glBindTexture(GL_TEXTURE_2D, brdf_shader_.brdf_lut_texture());
}
