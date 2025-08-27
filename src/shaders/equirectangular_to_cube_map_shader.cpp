#include "shaders/equirectangular_to_cube_map_shader.h"

#include "utils/utility.h"

EquirectangularToCubeMapShader::EquirectangularToCubeMapShader() {}

EquirectangularToCubeMapShader::EquirectangularToCubeMapShader(
    std::string fileVertexShader,
    std::string fileFragmentShader)
    : Shader(fileVertexShader, fileFragmentShader) {}

void EquirectangularToCubeMapShader::LoadTextures(const std::string& path) {
  hdr_texture_ = loadHdrTexture(path);
}

void EquirectangularToCubeMapShader::BindAllTextures() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, hdr_texture_);
}
