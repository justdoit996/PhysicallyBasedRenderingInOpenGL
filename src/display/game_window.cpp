#include "display/game_window.h"

#include <functional>
#include <iostream>

#include "utils/utility.h"

// Called whenever the window or framebuffer's size is changed
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

// 1. The first thing that is run when starting the window
void GameWindow::Initialize() {
  // Set GLFW stuff before window is created
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void GameWindow::SetUpMouseCallback() {
  // Set mouse
  glfwSetWindowUserPointer(this->windowHandle, this);
  auto func = [](GLFWwindow* w, double x, double y) {
    static_cast<GameWindow*>(glfwGetWindowUserPointer(w))
        ->MouseCallback(w, x, y);
  };
  glfwSetCursorPosCallback(this->windowHandle, func);
  // tell GLFW to capture our mouse
  glfwSetInputMode(this->windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// 2. Run after the window has been created, as well as the OpenGL context
void GameWindow::LoadContent() {
  // Set callback
  glfwSetFramebufferSizeCallback(this->windowHandle, FramebufferSizeCallback);
  SetUpMouseCallback();

  // TODO: Move this inside scene?
  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  // Initialize imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(this->windowHandle, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  std::cout << "INFO::IMGUI::SUCCESSFULLY_INITIALIZED" << std::endl;

  camera_ =
      std::make_unique<Camera>(/*starting position*/ glm::vec3(0, 0, 3.0));
  scene_ = std::make_unique<PbrScene>(camera_.get());
}

void GameWindow::Render() {
  // Before rendering, configure the viewport to the original framebuffer's
  // screen dimensions
  int scrWidth, scrHeight;
  glfwGetFramebufferSize(this->windowHandle, &scrWidth, &scrHeight);
  glViewport(0, 0, scrWidth, scrHeight);

  // Draw PBR scene
  scene_->Render();

  // Create new imgui frames
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Draw imgui
  ImGui::ShowDemoWindow(scene_.get());
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Swap double buffers and poll OS-events
  glfwSwapBuffers(this->windowHandle);
  glfwPollEvents();
}

void GameWindow::Update() {
  // Performs hot-reload of shader. Only reloads whenever it has been modified -
  // so not every frame.
  // sphere_shader_.ReloadFromFile();
  // environment_cube_map_shader_.ReloadFromFile();
  // equirectangular_to_cube_map_shader_.ReloadFromFile();
}

void GameWindow::Unload() {
  // Destroy imgui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void GameWindow::ProcessKeyboardInput() {
  // User input listener
  float current_time = glfwGetTime();
  delta_time_ = current_time - last_time_;
  last_time_ = current_time;
  if (glfwGetKey(this->windowHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(this->windowHandle, true);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_W) == GLFW_PRESS) {
    camera_->ProcessKeyboard(FORWARD, delta_time_);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_S) == GLFW_PRESS) {
    camera_->ProcessKeyboard(BACKWARD, delta_time_);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_A) == GLFW_PRESS) {
    camera_->ProcessKeyboard(LEFT, delta_time_);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_D) == GLFW_PRESS) {
    camera_->ProcessKeyboard(RIGHT, delta_time_);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_SPACE) == GLFW_PRESS) {
    camera_->ProcessKeyboard(UP, delta_time_);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    camera_->ProcessKeyboard(DOWN, delta_time_);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_F) == GLFW_PRESS &&
      !pause_key_pressed_) {
    pause_key_pressed_ = true;
    mouse_camera_enabled_ = !mouse_camera_enabled_;
    glfwSetInputMode(
        this->windowHandle, GLFW_CURSOR,
        mouse_camera_enabled_ ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_F) == GLFW_RELEASE) {
    pause_key_pressed_ = false;
  }
}

void GameWindow::MouseCallback(GLFWwindow* window, double x, double y) {
  if (!mouse_camera_enabled_) {
    first_mouse_ = true;
    return;
  }
  if (first_mouse_) {
    last_x_ = x;
    last_y_ = y;
    first_mouse_ = false;
  }

  float x_offset = x - last_x_;
  float y_offset = last_y_ - y;
  last_x_ = x;
  last_y_ = y;
  camera_->ProcessMouseMovement(x_offset, y_offset);
}
