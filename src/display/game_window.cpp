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

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // Initialize imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(this->windowHandle, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  std::cout << "INFO::IMGUI::SUCCESSFULLY_INITIALIZED" << std::endl;

  // Instance the camera
  camera_ = Camera(
      /*starting position*/ glm::vec3(0, 0, 3.0));

  // Shaders
  sphere_shader_ = SphereShader("resources/shaders/pbr/sphere.vs",
                                "resources/shaders/pbr/sphere.fs");
  equirectangular_to_cube_map_shader_ = EquirectangularToCubeMapShader(
      "resources/shaders/pbr/equirectangular.vs",
      "resources/shaders/pbr/equirectangular.fs");
  cube_map_shader_ = CubeMapShader("", "");

  // Bind projection uniform for camera shader (only need once)
  sphere_shader_.Use();
  camera_perspective_projection_ =
      glm::perspective(glm::radians(camera_.zoom()), constants::ASPECT_RATIO,
                       constants::NEAR, constants::FAR);
  sphere_shader_.SetMat4("projection", camera_perspective_projection_);

  // Load textures
  sphere_shader_.LoadTextures("resources/assets/textures/pbr/rusted_iron");
  equirectangular_to_cube_map_shader_.LoadTextures(
      "resources/assets/textures/hdr/newport_loft.hdr");

  // Create sphere vertices and VAO
  sphere_ = std::make_unique<Sphere>(/*sectors*/ 64, /*stacks*/ 64);
  cube_map_cube_ = std::make_unique<Cube>();

  // Light sources
  point_lights_.emplace_back(/*position*/ glm::vec3(0.0f, 0.0f, 10.0f),
                             /*color*/ glm::vec3(150.0f, 150.0f, 150.0f));
}

void GameWindow::Render() {
  // Clear the window
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  sphere_shader_.Use();
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = camera_.GetViewMatrix();
  sphere_shader_.SetMat4("model", model);
  sphere_shader_.SetMat4("view", view);
  sphere_shader_.SetVec3("cameraPos", camera_.position());
  sphere_shader_.BindAllTextures();
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

  // Create new imgui frames
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Draw imgui
  ImGui::ShowDemoWindow();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Swap double buffers and poll OS-events
  glfwSwapBuffers(this->windowHandle);
  glfwPollEvents();
}

void GameWindow::Update() {
  // Performs hot-reload of shader. Only reloads whenever it has been modified -
  // so not every frame.
  sphere_shader_.ReloadFromFile();
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
    camera_.ProcessKeyboard(FORWARD, delta_time_);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_S) == GLFW_PRESS) {
    camera_.ProcessKeyboard(BACKWARD, delta_time_);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_A) == GLFW_PRESS) {
    camera_.ProcessKeyboard(LEFT, delta_time_);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_D) == GLFW_PRESS) {
    camera_.ProcessKeyboard(RIGHT, delta_time_);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_SPACE) == GLFW_PRESS) {
    camera_.ProcessKeyboard(UP, delta_time_);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    camera_.ProcessKeyboard(DOWN, delta_time_);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_P) == GLFW_PRESS &&
      !pause_key_pressed_) {
    pause_key_pressed_ = true;
    mouse_camera_enabled_ = !mouse_camera_enabled_;
    glfwSetInputMode(
        this->windowHandle, GLFW_CURSOR,
        mouse_camera_enabled_ ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
  }
  if (glfwGetKey(this->windowHandle, GLFW_KEY_P) == GLFW_RELEASE) {
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
  camera_.ProcessMouseMovement(x_offset, y_offset);
}
