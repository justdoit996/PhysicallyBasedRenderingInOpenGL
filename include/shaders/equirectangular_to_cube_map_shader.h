#ifndef EQUIRECTANGULAR_TO_CUBE_MAP_SHADER_H
#define EQUIRECTANGULAR_TO_CUBE_MAP_SHADER_H

#include "shaders/shader.h"

#include <string>

// Parses an HDR texture map and binds it to be used later
class equirectangular_to_cube_map_shader : public Shader {
 public:
  equirectangular_to_cube_map_shader();
  equirectangular_to_cube_map_shader(std::string fileVertexShader,
                                     std::string fileFragmentShader);

  void LoadTextures(const std::string& path) override;
  void BindAllTextures() override;

 private:
  unsigned int hdr_texture_;
};
#endif