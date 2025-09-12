#include "shaders/irradiance_cube_map_shader.h"

#include "utils/constants.h"

IrradianceCubeMapShader::IrradianceCubeMapShader(std::string fileVertexShader,
                                                 std::string fileFragmentShader)
    : Shader(fileVertexShader, fileFragmentShader) {}

void IrradianceCubeMapShader::GenerateTextures() {
  glGenTextures(1, &irradiance_map_texture_);
  glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map_texture_);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                 constants::irradiance_map_dimension,
                 constants::irradiance_map_dimension, 0, GL_RGB, GL_FLOAT,
                 nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Texture uniforms
  this->Use();
  this->SetInt("environmentMap", 0);
}

void IrradianceCubeMapShader::BindAllTextures() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map_texture_);
}

unsigned int IrradianceCubeMapShader::irradiance_map_texture() {
  return this->irradiance_map_texture_;
}