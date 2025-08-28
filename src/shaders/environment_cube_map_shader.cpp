#include "shaders/environment_cube_map_shader.h"

EnvironmentCubeMapShader::EnvironmentCubeMapShader(
    std::string fileVertexShader,
    std::string fileFragmentShader)
    : Shader(fileVertexShader, fileFragmentShader) {}

void EnvironmentCubeMapShader::GenerateTextures() {
  glGenTextures(1, &env_cube_map_texture_);
  glBindTexture(GL_TEXTURE_CUBE_MAP, env_cube_map_texture_);

  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0,
                 GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Texture uniforms
  this->Use();
  this->SetInt("environmentCubeMap", 0);
}

void EnvironmentCubeMapShader::BindAllTextures() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, env_cube_map_texture_);
}

unsigned int EnvironmentCubeMapShader::env_cube_map_texture() {
  return this->env_cube_map_texture_;
}