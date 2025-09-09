#include "pbr_scene.h"

PbrScene::PbrScene(Camera* camera) : camera_(camera) {
  Init();
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

  // Load or generate textures
  sphere_shader_.LoadTextures("resources/assets/textures/pbr/rusted_iron");
  equirectangular_to_cube_map_shader_.LoadTextures(
      "resources/assets/textures/hdr/newport_loft.hdr");
  environment_cube_map_shader_.GenerateTextures();
  irradiance_cube_map_shader_.GenerateTextures();
  prefilter_shader_.GenerateTextures();
  brdf_shader_.GenerateTextures();

  // Create sphere vertices and VAO
  sphere_ = std::make_unique<Sphere>(/*sectors*/ 64, /*stacks*/ 64);
  cube_map_cube_ = std::make_unique<Cube>();
  quad_ = std::make_unique<Quad>();

  InitAllTextureMaps();

  // Light sources
  point_lights_.emplace_back(/*position*/ glm::vec3(0.0f, 0.0f, 10.0f),
                             /*color*/ glm::vec3(150.0f, 150.0f, 150.0f));

  // Bind projection uniform for camera shader (only need once)
  glm::mat4 camera_perspective_projection =
      glm::perspective(glm::radians(camera_->zoom()), constants::ASPECT_RATIO,
                       constants::NEAR, constants::FAR);
  sphere_shader_.Use();
  sphere_shader_.SetMat4("projection", camera_perspective_projection);
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
  // Adjust renderbuffer size to be 512x512 (magic number)
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
  equirectangular_to_cube_map_shader_.Use();
  equirectangular_to_cube_map_shader_.SetMat4("projection",
                                              capture_projection_);
  equirectangular_to_cube_map_shader_.BindAllTextures();
  glViewport(0, 0, 512, 512);
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
  // Adjust irradiance map dimensions to be 32x32
  glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo_);
  glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
  irradiance_cube_map_shader_.Use();
  irradiance_cube_map_shader_.SetMat4("projection", capture_projection_);
  environment_cube_map_shader_.BindAllTextures();
  glViewport(0, 0, 32, 32);
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
  unsigned int maxMipLevels = 5;
  for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
    // resize framebuffer according to mip-level size.
    unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
    unsigned int mipHeight =
        static_cast<unsigned int>(128 * std::pow(0.5, mip));
    glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth,
                          mipHeight);
    glViewport(0, 0, mipWidth, mipHeight);

    float roughness = (float)mip / (float)(maxMipLevels - 1);
    prefilter_shader_.SetFloat("roughness", roughness);
    for (unsigned int i = 0; i < 6; ++i) {
      prefilter_shader_.SetMat4("view", capture_views_[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             prefilter_shader_.prefilter_map_texture(), mip);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      cube_map_cube_->Draw();
    }
  }
}

void PbrScene::DrawBrdfIntegrationMap() {
  glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo_);
  glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         brdf_shader_.brdf_lut_texture(), 0);
  glViewport(0, 0, 512, 512);
  brdf_shader_.Use();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  quad_->Draw();
}

void PbrScene::Render() {
  // Clear the window
  // black
  // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  // cyan
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = camera_->GetViewMatrix();

  sphere_shader_.Use();
  sphere_shader_.SetMat4("model", model);
  sphere_shader_.SetMat4("view", view);
  sphere_shader_.SetVec3("cameraPos", camera_->position());
  sphere_shader_.BindAllTextures();
  // TODO: BUG fix texture mapping overwriting for environmentMap
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_CUBE_MAP,
                irradiance_cube_map_shader_.irradiance_map_texture());
  sphere_->Draw();

  // Light sources
  // Use sphere shader as a light source for testing
  for (int i = 0; i < point_lights_.size(); i++) {
    glm::vec3 newPos = point_lights_[i].position +
                       glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
    // newPos = point_lights_[i].position;
    sphere_shader_.SetVec3("pointLights[" + std::to_string(i) + "].Position",
                           newPos);
    sphere_shader_.SetVec3("pointLights[" + std::to_string(i) + "].Color",
                           point_lights_[i].color);
    model = glm::mat4(1.0f);
    model = glm::translate(model, newPos);
    model = glm::scale(model, glm::vec3(0.5f));
    sphere_shader_.SetMat4("model", model);
    sphere_shader_.SetMat3("normalMatrix",
                           glm::transpose(glm::inverse(glm::mat3(model))));
    sphere_->Draw();
  }

  // Render skybox (render background last to prevent overdrawing)
  environment_cube_map_shader_.Use();
  environment_cube_map_shader_.SetMat4("view", view);
  environment_cube_map_shader_.BindAllTextures();
  //  prefilter_shader_.Use();
  //  prefilter_shader_.SetMat4("view", view);
  // prefilter_shader_.BindAllTextures();
  cube_map_cube_->Draw();
}
