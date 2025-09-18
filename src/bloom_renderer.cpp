#include "bloom_renderer.h"

BloomRenderer::BloomRenderer() : init_(false) {}

BloomRenderer::~BloomRenderer() {}

bool BloomRenderer::Init() {
  if (init_) {
    return true;
  }
  // Framebuffer
  bool status = bloom_framebuffer_.Init(/*max mip levels*/ 6);
  if (!status) {
    std::cerr
        << "Failed to initialize bloom FBO - cannot create bloom renderer!\n";
    return false;
  }

  // Shaders
  down_sample_shader_ = Shader("resources/shaders/pbr/down_sample.vs",
                               "resources/shaders/pbr/down_sample.fs");
  up_sample_shader_ = Shader("resources/shaders/pbr/up_sample.vs",
                             "resources/shaders/pbr/up_sample.fs");

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

void BloomRenderer::Destroy() {
  bloom_framebuffer_.Destroy();
}

void BloomRenderer::RenderBloomTexture(unsigned int srcTexture,
                                       float filterRadius) {
  bloom_framebuffer_.BindForWriting();

  this->RenderDownsamples(srcTexture);
  this->RenderUpsamples(filterRadius);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // Restore viewport
  glViewport(0, 0, src_viewport_size_.x, src_viewport_size_.y);
}

void BloomRenderer::RenderDownsamples(unsigned int srcTexture) {
  const std::vector<BloomFramebuffer::Mip>& mipChain =
      bloom_framebuffer_.MipChain();

  down_sample_shader_.Use();
  down_sample_shader_.SetVec2("srcResolution", src_viewport_size_float_);
  if (mKarisAverageOnDownsample) {
    down_sample_shader_.SetInt("mipLevel", 0);
  }

  // Bind srcTexture (HDR color buffer) as initial texture input
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, srcTexture);

  // Progressively downsample through the mip chain
  for (int i = 0; i < (int)mipChain.size(); i++) {
    const BloomFramebuffer::Mip& mip = mipChain[i];
    glViewport(0, 0, mip.size.x, mip.size.y);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           mip.texture, 0);

    // Render screen-filled quad of resolution of current mip
    quad_.Draw();

    // Set current mip resolution as srcResolution for next iteration
    down_sample_shader_.SetVec2("srcResolution", mip.size);
    // Set current mip as texture input for next iteration
    glBindTexture(GL_TEXTURE_2D, mip.texture);
    // Disable Karis average for consequent downsamples
    if (i == 0) {
      down_sample_shader_.SetInt("mipLevel", 1);
    }
  }

  glUseProgram(0);
}

void BloomRenderer::RenderUpsamples(float filterRadius) {
  const std::vector<BloomFramebuffer::Mip>& mipChain =
      bloom_framebuffer_.MipChain();

  up_sample_shader_.Use();
  up_sample_shader_.SetFloat("filterRadius", filterRadius);

  // Enable additive blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  glBlendEquation(GL_FUNC_ADD);

  for (int i = (int)mipChain.size() - 1; i > 0; i--) {
    const BloomFramebuffer::Mip& mip = mipChain[i];
    const BloomFramebuffer::Mip& nextMip = mipChain[i - 1];

    // Bind viewport and texture from where to read
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mip.texture);

    // Set framebuffer render target (we write to this texture)
    glViewport(0, 0, nextMip.size.x, nextMip.size.y);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           nextMip.texture, 0);

    // Render screen-filled quad of resolution of current mip
    quad_.Draw();
  }

  // Disable additive blending
  // glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_BLEND);

  glUseProgram(0);
}

unsigned int BloomRenderer::BloomTexture() {
  return bloom_framebuffer_.MipChain()[0].texture;
}

unsigned int BloomRenderer::BloomMip_i(int index) {
  const std::vector<BloomFramebuffer::Mip>& mipChain =
      bloom_framebuffer_.MipChain();
  int size = (int)mipChain.size();
  return mipChain[index].texture;
}
