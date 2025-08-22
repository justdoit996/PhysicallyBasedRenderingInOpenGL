#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include "display/game_window.h"

int main() {
  // Create the game window with the specified size and title
  GameWindow gw =
      GameWindow{1500, 1000, "opengl-imgui-cmake-template by @dcronqvist"};
  return gw.Run();
}