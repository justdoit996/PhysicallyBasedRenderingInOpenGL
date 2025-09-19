#include "pbr_scene.h"

#include "bloom_renderer.h"
#include "ui_to_scene_data.h"
#include "utils/constants.h"

PbrScene::PbrScene(Camera* camera) : camera_(camera) {
  Init();
}

void PbrScene::UploadPbrTextures(const std::string& path) {
  sphere_shader_.LoadTextures(path);
}

void PbrScene::UploadHdrMap(const std::string& path) {
  ibl_renderer_.UploadHdrMap(path);
}

void PbrScene::InitAllIblTextureMaps() {
  ibl_renderer_.InitAllTextureMaps();
}

void PbrScene::Init() {
  // Shaders
  sphere_shader_ = SphereShader("resources/shaders/pbr/sphere.vs",
                                "resources/shaders/pbr/sphere.fs");
  light_sphere_shader_ =
      Shader("resources/shaders/light_sphere/light_sphere.vs",
             "resources/shaders/light_sphere/light_sphere.fs");
  framebuffer_to_screen_shader_ =
      FramebufferShader("resources/shaders/pbr/framebuffer.vs",
                        "resources/shaders/pbr/framebuffer.fs");

  // Load or generate textures
  sphere_shader_.LoadTextures(
      pbr_utils::ConvertMaterialToFilePath(pbr_utils::materials[0]));
  framebuffer_to_screen_shader_.GenerateTextures();

  // Create sphere vertices and VAO
  sphere_ = std::make_unique<Sphere>(/*sectors*/ 64, /*stacks*/ 64);
  quad_ = std::make_unique<Quad>();

  // Pre-render all texture maps and LUTs for IBL
  InitAllIblTextureMaps();

  // bloom renderer
  bloom_renderer_.Init();

  // Light sources
  point_light_ = PointLight(
      /*position*/ glm::vec3(0.0f, 0.0f, 0.0f),
      /*color*/
      glm::vec3(pbr_utils::ui_defaults::light_sphere::redf,
                pbr_utils::ui_defaults::light_sphere::greenf,
                pbr_utils::ui_defaults::light_sphere::bluef),
      /*intensity*/ 200.f);

  // Bind projection uniform for camera shader (only need once)
  glm::mat4 camera_perspective_projection =
      glm::perspective(constants::camera::ZOOM, constants::ASPECT_RATIO,
                       constants::camera::NEAR, constants::camera::FAR);
  sphere_shader_.Use();
  sphere_shader_.SetMat4("projection", camera_perspective_projection);
  light_sphere_shader_.Use();
  light_sphere_shader_.SetMat4("projection", camera_perspective_projection);
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
  // TODO: synchronize the binding of these textures to same shader file
  sphere_shader_.BindAllTextures();
  ibl_renderer_.BindAllTextures();

  // New position for light cube
  float radius_of_revolution = 5.f;
  glm::vec3 newPos =
      point_light_.position +
      glm::vec3(
          point_light_.position.x + radius_of_revolution * cos(glfwGetTime()),
          0,
          point_light_.position.z + radius_of_revolution * sin(glfwGetTime()));
  // Add direct light properties in sphere shader
  sphere_shader_.SetBool("pointLightEnabled", point_light_enabled_);
  sphere_shader_.SetVec3("pointLight.Position", newPos);
  sphere_shader_.SetVec3("pointLight.Color", point_light_.GetTotalColor());
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

  ibl_renderer_.RenderEnvironment(view);

  // NOT OPTIONAL STEP: Clear the framebuffer!!
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Create bloom mip maps after scene has been drawn to framebuffer
  if (bloom_enabled_) {
    bloom_renderer_.RenderBloomTexture(
        framebuffer_to_screen_shader_.color_buffer(1),
        constants::bloom_filter_radius);
  }

  // Use framebuffer shader to draw from framebuffer to screen
  // NOT OPTIONAL STEP: Clear the framebuffer!!
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  framebuffer_to_screen_shader_.Use();
  // Bind the original color texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, framebuffer_to_screen_shader_.color_buffer(0));
  // If bloom enabled, bind the post-bloom texture
  // else bind nothing (same as default)
  glActiveTexture(GL_TEXTURE1);
  if (bloom_enabled_) {
    glBindTexture(GL_TEXTURE_2D, bloom_renderer_.BloomTexture());
  } else {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  // TODO: slider for HDR exposure?
  framebuffer_to_screen_shader_.SetFloat("exposure", 1.f);
  framebuffer_to_screen_shader_.SetBool("bloomEnabled", bloom_enabled_);
  framebuffer_to_screen_shader_.SetFloat("bloomStrength", bloom_strength_);
  quad_->Draw();
}

void PbrScene::SetPointLightEnabled(bool enable) {
  point_light_enabled_ = enable;
}

void PbrScene::SetRedColor(float r) {
  point_light_.color.r = r;
}

void PbrScene::SetGreenColor(float g) {
  point_light_.color.g = g;
}

void PbrScene::SetBlueColor(float b) {
  point_light_.color.b = b;
}

void PbrScene::SetLightIntensity(float intense) {
  point_light_.intensity = intense;
}

void PbrScene::SetBloomEnabled(bool bloom) {
  bloom_enabled_ = bloom;
}

void PbrScene::SetBloomStrength(float str) {
  bloom_strength_ = str;
}
