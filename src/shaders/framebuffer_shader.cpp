#include "shaders/framebuffer_shader.h"

#include "utils/constants.h"

FramebufferShader::FramebufferShader(const std::string& path_to_vertex_shader,
                                     const std::string& path_to_frag_shader)
    : Shader(path_to_vertex_shader, path_to_frag_shader) {}

void FramebufferShader::GenerateTextures() {
  // configure (floating point) framebuffers
  // ---------------------------------------
  glGenFramebuffers(1, &fbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glGenTextures(2, color_buffers_);
  for (unsigned int i = 0; i < 2; i++) {
    glBindTexture(GL_TEXTURE_2D, color_buffers_[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, constants::WIDTH,
                 constants::HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would
                            // otherwise sample repeated texture values!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                           GL_TEXTURE_2D, color_buffers_[i], 0);
  }
  // create and attach depth buffer (renderbuffer)
  glGenRenderbuffers(1, &rbo_depth_);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, constants::WIDTH,
                        constants::HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, rbo_depth_);
  // tell OpenGL which color attachments we'll use (of this framebuffer) for
  // rendering
  unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
  glDrawBuffers(2, attachments);
  // finally check if framebuffer is complete
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Framebuffer not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  this->Use();
  this->SetInt("scene", 0);
  this->SetInt("bloomBlur", 1);
}

void FramebufferShader::BindAllTextures() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, color_buffers_[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, color_buffers_[1]);
}

void FramebufferShader::BindFramebuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}
