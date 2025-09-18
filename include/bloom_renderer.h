#ifndef BLOOM_RENDERER_H
#define BLOOM_RENDERER_H

#include "bloom_framebuffer.h"
#include "shaders/shader.h"

class BloomRenderer : public Shader {
 public:
  BloomRenderer();
  ~BloomRenderer();
  bool Init(unsigned int windowWidth, unsigned int windowHeight);
  void Destroy();
  void RenderBloomTexture(unsigned int srcTexture, float filterRadius);
  unsigned int BloomTexture();
  unsigned int BloomMip_i(int index);

 private:
  void RenderDownsamples(unsigned int srcTexture);
  void RenderUpsamples(float filterRadius);

  bool mInit;
  BloomFramebuffer mFBO;
  glm::ivec2 mSrcViewportSize;
  glm::vec2 mSrcViewportSizeFloat;
  Shader* mDownsampleShader;
  Shader* mUpsampleShader;

  bool mKarisAverageOnDownsample = true;
};

#endif