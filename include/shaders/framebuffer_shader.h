#include "shaders/shader.h"

#include <string>

class FramebufferShader : public Shader {
 public:
  FramebufferShader(const std::string& path_to_vertex_shader,
                    const std::string& path_to_frag_shader);
  FramebufferShader() = default;

  void GenerateTextures() override;
  void BindFramebuffer();
  unsigned int color_buffer(int idx) { return color_buffers_[idx]; }

 private:
  unsigned int fbo_ = 0;
  // create 2 floating point color buffers
  // 0 for normal rendering
  // 1 for brightness threshold values
  unsigned int color_buffers_[2] = {};
  unsigned int rbo_depth_ = 0;
};