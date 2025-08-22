#include "display/base_window.h"

#include "camera.h"

class GameWindow : public BaseWindow {
 public:
  GameWindow(int width, int height, std::string title)
      : BaseWindow(width, height, title) {};
  void Initialize();
  void LoadContent();
  void Update();
  void Render();
  void Unload();
  void ProcessKeyboardInput() override;

 private:
  Camera camera_;
};