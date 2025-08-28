#ifndef EQUIRECTANGULAR_TO_CUBE_MAP_SHADER_H
#define EQUIRECTANGULAR_TO_CUBE_MAP_SHADER_H

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include "shaders/shader.h"

#include <string>

// Parses an HDR texture map and binds it to be used later
class EquirectangularToCubeMapShader : public Shader {
 public:
  EquirectangularToCubeMapShader();
  EquirectangularToCubeMapShader(const std::string& fileVertexShader,
                                 const std::string& fileFragmentShader);

  void LoadTextures(const std::string& path) override;
  void BindAllTextures() override;

  unsigned int hdr_texture_;

 private:
};
#endif