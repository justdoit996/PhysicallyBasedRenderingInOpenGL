#include "shaders/equirectangular_to_cube_map_shader.h"

#include "utils/utility.h"

EquirectangularToCubeMapShader::EquirectangularToCubeMapShader() {}

EquirectangularToCubeMapShader::EquirectangularToCubeMapShader(
    const std::string& fileVertexShader,
    const std::string& fileFragmentShader)
    : Shader(fileVertexShader, fileFragmentShader) {}

void EquirectangularToCubeMapShader::LoadTextures(const std::string& path) {
  hdr_texture_ = LoadHdrTexture(path);

  // Texture uniforms
  this->Use();
  this->SetInt("equirectangularMap", 0);
}

void EquirectangularToCubeMapShader::BindAllTextures() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, hdr_texture_);
}
