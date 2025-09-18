#ifndef BLOOM_FRAMEBUFFER_H
#define BLOOM_FRAMEBUFFER_H

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include <glm/glm.hpp>
#include <vector>

// Class to manage bloom framebuffer for up/down sampling
class BloomFbo {
 public:
  BloomFbo();
  ~BloomFbo();

  // struct that represents one "layer" of a mip map
  struct Mip {
    glm::vec2 size;
    glm::ivec2 intSize;
    unsigned int texture;
  };

  bool Init(unsigned int mipChainLength);
  void Destroy();
  void BindForWriting();
  const std::vector<Mip>& MipChain() const;

 private:
  bool init_;
  unsigned int fbo_;
  // Holds all mip map layers in a sequence
  std::vector<Mip> mip_chain_;
};

#endif