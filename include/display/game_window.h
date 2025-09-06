#include "display/base_window.h"

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

class GameWindow : public BaseWindow {
 public:
  GameWindow(int width, int height, std::string title)
      : BaseWindow(width, height, title) {};
  void Initialize();
  void LoadContent();
  void Update();
  void Render();
  void Unload();

 private:
  void ProcessKeyboardInput() override;
  void SetUpMouseCallback();
  void MouseCallback(GLFWwindow* window, double x, double y);

  // Converts HDR equirectangular environment map to cubemap equivalent
  void DrawCubeMapToFramebuffer();

  // Camera
  Camera camera_;
  glm::mat4 camera_perspective_projection_;

  // Shapes for generating, binding, or drawing vertices
  std::unique_ptr<Sphere> sphere_;
  std::unique_ptr<Cube> cube_map_cube_;
  std::unique_ptr<Quad> quad_;

  // Shaders
  SphereShader sphere_shader_;
  EquirectangularToCubeMapShader equirectangular_to_cube_map_shader_;
  EnvironmentCubeMapShader environment_cube_map_shader_;
  IrradianceCubeMapShader irradiance_cube_map_shader_;
  PrefilterShader prefilter_shader_;

  // Lights
  std::vector<PointLight> point_lights_;

  // Keyboard input time delta
  float delta_time_ = 0.0f;
  float last_time_ = 0.0f;
  bool first_mouse_ = true;
  bool mouse_camera_enabled_ = true;
  bool pause_key_pressed_ = false;
  float last_x_ = constants::WIDTH / 2.f;
  float last_y_ = constants::HEIGHT / 2.f;
};