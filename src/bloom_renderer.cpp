#include "bloom_renderer.h"

BloomRenderer::BloomRenderer() : init_(false) {}

BloomRenderer::~BloomRenderer() {}

bool BloomRenderer::Init() {
  if (init_) {
    return true;
  }
  // Framebuffer
  const unsigned int num_bloom_mips = 6;  // TODO: Play around with this value
  bool status = bloom_framebuffer_.Init(num_bloom_mips);
  if (!status) {
    std::cerr
        << "Failed to initialize bloom FBO - cannot create bloom renderer!\n";
    return false;
  }

  // Shaders
  down_sample_shader_ = Shader("resources/shaders/pbr/down_sample.vs",
                               "resources/shaders/pbr/down_sample.fs");
  up_sample_shader_ = Shader("6.new_upsample.vs", "6.new_upsample.fs");

  // Downsample
  down_sample_shader_.Use();
  down_sample_shader_.SetInt("srcTexture", 0);
  glUseProgram(0);

  // Upsample
  up_sample_shader_.Use();
  up_sample_shader_.SetInt("srcTexture", 0);
  glUseProgram(0);

  return true;
}

void BloomRenderer::Destroy() {}
