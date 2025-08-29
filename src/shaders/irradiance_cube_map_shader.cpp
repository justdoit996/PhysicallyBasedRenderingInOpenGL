#include "shaders/irradiance_cube_map_shader.h"

IrradianceCubeMapShader::IrradianceCubeMapShader(std::string fileVertexShader,
                                                 std::string fileFragmentShader)
    : Shader(fileVertexShader, fileFragmentShader) {}

void IrradianceCubeMapShader::GenerateTextures() {
  // Texture uniforms
  this->Use();
  this->SetInt("irradianceMap", 0);
}

void IrradianceCubeMapShader::BindAllTextures() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map_texture_);
}

unsigned int IrradianceCubeMapShader::irradiance_map_texture() {
  return this->irradiance_map_texture_;
}