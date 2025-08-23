#include "display/base_window.h"

#include <glm/glm.hpp>
#include <memory>

#include "camera.h"
#include "shader.h"
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

  // Camera
  Camera camera_;
  glm::mat4 camera_perspective_projection_;

  Shader sphere_shader_;
  std::unique_ptr<Sphere> sphere_;
  unsigned int albedo_;
  unsigned int ao_;

  // Keyboard input time delta
  float delta_time_ = 0.0f;
  float last_time_ = 0.0f;
  bool first_mouse_ = true;
  bool mouse_camera_enabled_ = true;
  bool pause_key_pressed_ = false;
  float last_x_ = constants::WIDTH / 2.f;
  float last_y_ = constants::HEIGHT / 2.f;
};