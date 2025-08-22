#include "display/game_window.h"

#include <iostream>

#include "shaders/shader.h"
#include "utils/constants.h"

// Template stuff
Shader shader;
unsigned int VAO;
unsigned int VBO;
unsigned int EBO;

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

  camera_ = Camera(/*starting position*/ glm::vec3(0, 0, 3.0));
}

// 2. Run after the window has been created, as well as the OpenGL context
void GameWindow::LoadContent() {
  // Set callback
  glfwSetFramebufferSizeCallback(this->windowHandle, FramebufferSizeCallback);

  // Initialize imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(this->windowHandle, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  std::cout << "INFO::IMGUI::SUCCESSFULLY_INITIALIZED" << std::endl;

  // Load the template shader
  shader = Shader::LoadShader("resources/shaders/testing.vs",
                              "resources/shaders/testing.fs");

  // Vertices needed for a square
  float vertices[] = {
      0.5f,  0.5f,  0.0f,  // top right
      0.5f,  -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f   // top left
  };

  // Indices for rendering the above square
  unsigned int indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };

  // Create Vertex Array object
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);  // And bind it

  // Create Vertex Buffer object
  glGenBuffers(1, &VBO);
  // And bind it (this also includes it into the VAO)
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Fill the VBO with vertex data, simply positions
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // layout = 0 should contain these positions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);  // Enable that shit

  // Create index buffer
  glGenBuffers(1, &EBO);
  // And bind it (also included in VAO)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // Fill with indices!
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
}

void GameWindow::Update() {
  // Performs hot-reload of shader. Only reloads whenever it has been modified -
  // so not every frame.
  shader.ReloadFromFile();
}

void GameWindow::Render() {
  // Bind the VAO
  glBindVertexArray(VAO);

  // Make sure we're using the correct shader program.
  // Must be done per-frame, since the shader program id might change when
  // hot-reloading
  shader.Use();
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = camera_.GetViewMatrix();
  glm::mat4 projection =
      glm::perspective(glm::radians(camera_.zoom()),
                       (float)constants::ASPECT_RATIO, 0.1f, 100.0f);
  shader.SetMat4("model", model);
  shader.SetMat4("view", view);
  shader.SetMat4("projection", projection);
  shader.SetVec3("cameraPos", camera_.position());

  // Create new imgui frames
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Clear the window
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw the square
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  // Draw imgui
  ImGui::ShowDemoWindow();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Swap double buffers and poll OS-events
  glfwSwapBuffers(this->windowHandle);
  glfwPollEvents();
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
}