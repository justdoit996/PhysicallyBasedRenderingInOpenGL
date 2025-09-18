#ifndef BLOOM_RENDERER_H
#define BLOOM_RENDERER_H

#include <glm/glm.hpp>
#include "bloom_framebuffer.h"
#include "shaders/shader.h"
#include "utils/constants.h"

class BloomRenderer {
 public:
  BloomRenderer();
  ~BloomRenderer();
  bool Init();
  void Destroy();
  void RenderBloomTexture(unsigned int srcTexture, float filterRadius);
  unsigned int BloomTexture();
  unsigned int BloomMip_i(int index);

 private:
  void RenderDownsamples(unsigned int srcTexture);
  void RenderUpsamples(float filterRadius);

  bool init_;
  BloomFramebuffer bloom_framebuffer_;
  glm::ivec2 src_viewport_size_ =
      glm::ivec2(constants::WIDTH, constants::HEIGHT);
  glm::vec2 src_viewport_size_float_ =
      glm::vec2((float)constants::WIDTH, (float)constants::HEIGHT);

  Shader down_sample_shader_;
  Shader up_sample_shader_;

  bool mKarisAverageOnDownsample = true;
};

#endif