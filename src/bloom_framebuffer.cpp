#include "bloom_framebuffer.h"

#include <iostream>
#include "utils/constants.h"

BloomFramebuffer::BloomFramebuffer() : init_(false), fbo_(0) {}

BloomFramebuffer::~BloomFramebuffer() {}

bool BloomFramebuffer::Init(unsigned int mipChainLength) {
  if (init_) {
    return true;
  }

  glGenFramebuffers(1, &fbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

  glm::vec2 mipSize((float)constants::WIDTH, (float)constants::HEIGHT);
  glm::ivec2 mipIntSize((int)constants::WIDTH, (int)constants::HEIGHT);

  for (GLuint i = 0; i < mipChainLength; i++) {
    mipSize *= 0.5f;
    mipIntSize /= 2;
    BloomFramebuffer::Mip mip{mipSize, mipIntSize};

    glGenTextures(1, &mip.texture);
    glBindTexture(GL_TEXTURE_2D, mip.texture);
    // we are downscaling an HDR color buffer, so we need a float texture format
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, (int)mipSize.x,
                 (int)mipSize.y, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    std::cout << "Created bloom mip " << mipIntSize.x << 'x' << mipIntSize.y
              << std::endl;
    mip_chain_.emplace_back(mip);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         mip_chain_[0].texture, 0);

  // setup attachments
  unsigned int attachments[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, attachments);

  // check completion status
  int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Bloom framebuffer status incomplete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  init_ = true;
  return true;
}

void BloomFramebuffer::Destroy() {
  for (int i = 0; i < (int)mip_chain_.size(); i++) {
    glDeleteTextures(1, &mip_chain_[i].texture);
    mip_chain_[i].texture = 0;
  }
  glDeleteFramebuffers(1, &fbo_);
  fbo_ = 0;
  init_ = false;
}

void BloomFramebuffer::BindForWriting() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

const std::vector<BloomFramebuffer::Mip>& BloomFramebuffer::MipChain() const {
  return mip_chain_;
}
