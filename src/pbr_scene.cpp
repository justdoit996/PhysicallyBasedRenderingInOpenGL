#include "pbr_scene.h"

#include "bloom_framebuffer.h"
#include "ui_to_scene_data.h"
#include "utils/constants.h"

PbrScene::PbrScene(Camera* camera) : camera_(camera) {
  Init();
}

void PbrScene::UploadPbrTextures(const std::string& path) {
  sphere_shader_.LoadTextures(path);
}

void PbrScene::UploadHdrMap(const std::string& path) {
  equirectangular_to_cube_map_shader_.LoadTextures(path);
}

void PbrScene::Init() {
  // Shaders
  sphere_shader_ = SphereShader("resources/shaders/pbr/sphere.vs",
                                "resources/shaders/pbr/sphere.fs");
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
  light_sphere_shader_ =
      Shader("resources/shaders/light_sphere/light_sphere.vs",
             "resources/shaders/light_sphere/light_sphere.fs");
  framebuffer_to_screen_shader_ =
      FramebufferShader("resources/shaders/pbr/framebuffer.vs",
                        "resources/shaders/pbr/framebuffer.fs");

  // Load or generate textures
  UploadPbrTextures(
      pbr_utils::ConvertMaterialToFilePath(pbr_utils::materials[0]));
  UploadHdrMap(
      pbr_utils::ConvertEnvironmentToFilePath(pbr_utils::environments[0]));
  environment_cube_map_shader_.GenerateTextures();
  irradiance_cube_map_shader_.GenerateTextures();
  prefilter_shader_.GenerateTextures();
  brdf_shader_.GenerateTextures();
  framebuffer_to_screen_shader_.GenerateTextures();

  // Create sphere vertices and VAO
  sphere_ = std::make_unique<Sphere>(/*sectors*/ 64, /*stacks*/ 64);
  cube_map_cube_ = std::make_unique<Cube>();
  quad_ = std::make_unique<Quad>();

  InitAllTextureMaps();

  // Light sources
  point_light_ = PointLight(/*position*/ glm::vec3(0.0f, 0.0f, 0.0f),
                            /*color*/ glm::vec3(1.0f, 1.0f, 1.0f));
  point_light_.SetIntensity(200.f);

  // Bind projection uniform for camera shader (only need once)
  glm::mat4 camera_perspective_projection =
      glm::perspective(glm::radians(camera_->zoom()), constants::ASPECT_RATIO,
                       constants::NEAR, constants::FAR);
  sphere_shader_.Use();
  sphere_shader_.SetMat4("projection", camera_perspective_projection);
  light_sphere_shader_.Use();
  light_sphere_shader_.SetMat4("projection", camera_perspective_projection);
  environment_cube_map_shader_.Use();
  environment_cube_map_shader_.SetMat4("projection",
                                       camera_perspective_projection);
}

void PbrScene::InitAllTextureMaps() {
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

void PbrScene::CreateAndBindFramebufferAndRenderBufferObjects() {
  // Create and bind framebuffer and renderbuffer
  glGenFramebuffers(1, &capture_fbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo_);
  glGenRenderbuffers(1, &capture_rbo_);
  glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo_);
  // Attach renderbuffer to framebuffer
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, capture_rbo_);
}

void PbrScene::ConvertEquirectangularTextureToCubeMap() {
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

void PbrScene::DrawIrradianceMap() {
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

void PbrScene::DrawPreFilteredEnvironmentMap() {
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
                             prefilter_shader_.prefilter_map_texture(), i);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      cube_map_cube_->Draw();
    }
  }
}

void PbrScene::DrawBrdfIntegrationMap() {
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

void PbrScene::Render() {
  // Clear the window
  // black
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  // cyan
  // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = camera_->GetViewMatrix();

  // Draw the whole scene to a framebuffer
  framebuffer_to_screen_shader_.BindFramebuffer();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  sphere_shader_.Use();
  sphere_shader_.SetMat4("model", model);
  sphere_shader_.SetMat4("view", view);
  sphere_shader_.SetVec3("cameraPos", camera_->position());
  sphere_shader_.BindAllTextures();
  // TODO: Fix manual texture mapping overwriting
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_CUBE_MAP,
                irradiance_cube_map_shader_.irradiance_map_texture());
  glActiveTexture(GL_TEXTURE6);
  glBindTexture(GL_TEXTURE_CUBE_MAP, prefilter_shader_.prefilter_map_texture());
  glActiveTexture(GL_TEXTURE7);
  glBindTexture(GL_TEXTURE_2D, brdf_shader_.brdf_lut_texture());

  // New position for light cube
  float radius_of_revolution = 5.f;
  glm::vec3 newPos =
      point_light_.position +
      glm::vec3(
          point_light_.position.x + radius_of_revolution * cos(glfwGetTime()),
          0,
          point_light_.position.z + radius_of_revolution * sin(glfwGetTime()));
  // Add direct light properties in sphere shader
  // TODO: Toggle on/off point light source
  sphere_shader_.SetBool("pointLightEnabled", point_light_enabled_);
  sphere_shader_.SetVec3("pointLight.Position", newPos);
  sphere_shader_.SetVec3("pointLight.Color", point_light_.GetColorLuminance());
  sphere_shader_.SetMat3("normalMatrix",
                         glm::transpose(glm::inverse(glm::mat3(model))));
  sphere_->Draw();

  // Render the light cube
  if (point_light_enabled_) {
    light_sphere_shader_.Use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, newPos);
    model = glm::scale(model, glm::vec3(.5f));
    light_sphere_shader_.SetMat4("model", model);
    light_sphere_shader_.SetMat4("view", view);
    light_sphere_shader_.SetVec3("light_color", point_light_.GetColor());
    sphere_->Draw();
  }

  // Render skybox (render background last to prevent overdrawing)
  environment_cube_map_shader_.Use();
  environment_cube_map_shader_.SetMat4("view", view);
  environment_cube_map_shader_.BindAllTextures();
  cube_map_cube_->Draw();

  // NOT OPTIONAL STEP: Clear the framebuffer!!
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Draw the scene from framebuffer to screen
  framebuffer_to_screen_shader_.Use();
  framebuffer_to_screen_shader_.BindAllTextures();
  framebuffer_to_screen_shader_.SetFloat("exposure", 1.f);
  quad_->Draw();
}

void PbrScene::SetPointLightEnabled(bool enable) {
  point_light_enabled_ = enable;
}

void PbrScene::SetRedColor(unsigned int r) {
  point_light_.color.r = r / 255.f;
}

void PbrScene::SetGreenColor(unsigned int g) {
  point_light_.color.g = g / 255.f;
}

void PbrScene::SetBlueColor(unsigned int b) {
  point_light_.color.b = b / 255.f;
}

void PbrScene::SetLightIntensity(float intense) {
  point_light_.SetIntensity(intense);
}
