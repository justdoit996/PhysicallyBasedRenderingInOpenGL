#include "shaders/cube_map_convolution_shader.h"

CubeMapConvolutionShader::CubeMapConvolutionShader(
    std::string fileVertexShader,
    std::string fileFragmentShader)
    : Shader(fileVertexShader, fileFragmentShader) {}

void CubeMapConvolutionShader::GenerateTextures() {
  unsigned int irradiance_map_texture_;
  glGenTextures(1, &irradiance_map_texture_);
  glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map_texture_);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0,
                 GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void CubeMapConvolutionShader::BindAllTextures() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map_texture_);
}
