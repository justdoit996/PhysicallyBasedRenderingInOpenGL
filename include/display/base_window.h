#ifndef DISPLAY_BASE_WINDOW_H
#define DISPLAY_BASE_WINDOW_H

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include <string>
#include "glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class BaseWindow {
 public:
  int windowWidth, windowHeight;
  std::string windowTitle;
  GLFWwindow* windowHandle;

 public:
  BaseWindow();
  BaseWindow(int width, int height, std::string title);
  int Run();

 protected:
  virtual void Initialize() = 0;
  virtual void LoadContent() = 0;
  virtual void Update() = 0;
  virtual void Render() = 0;
  virtual void Unload() = 0;
};

#endif