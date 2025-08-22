#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include "display/game_window.h"
#include "utils/constants.h"

int main() {
  // Create the game window with the specified size and title
  GameWindow gw =
      GameWindow{constants::WIDTH, constants::HEIGHT, "PBRinOpenGL"};
  return gw.Run();
}