#include "shaders/equirectangular_to_cube_map_shader.h"

#include "utils/utility.h"

equirectangular_to_cube_map_shader::equirectangular_to_cube_map_shader() {}

equirectangular_to_cube_map_shader::equirectangular_to_cube_map_shader(
    std::string fileVertexShader,
    std::string fileFragmentShader)
    : Shader(fileVertexShader, fileFragmentShader) {}

void equirectangular_to_cube_map_shader::LoadTextures(const std::string& path) {
  hdr_texture_ = loadHdrTexture(path);
}

void equirectangular_to_cube_map_shader::BindAllTextures() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, hdr_texture_);
}
