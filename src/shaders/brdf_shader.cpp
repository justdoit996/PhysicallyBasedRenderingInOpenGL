#include "shaders/brdf_shader.h"

BrdfShader::BrdfShader(std::string fileVertexShader,
                       std::string fileFragmentShader)
    : Shader(fileVertexShader, fileFragmentShader) {}

void BrdfShader::GenerateTextures() {
  glGenTextures(1, &brdf_lut_texture_);
  glBindTexture(GL_TEXTURE_2D, brdf_lut_texture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 1024, 1024, 0, GL_RG, GL_FLOAT, 0);
  // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void BrdfShader::BindAllTextures() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, brdf_lut_texture_);
}
