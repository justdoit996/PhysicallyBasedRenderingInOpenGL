#ifndef ENVIRONMENT_CUBE_MAP_SHADER_H
#define ENVIRONMENT_CUBE_MAP_SHADER_H

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include "shaders/shader.h"

class EnvironmentCubeMapShader : public Shader {
 public:
  EnvironmentCubeMapShader() = default;
  EnvironmentCubeMapShader(std::string fileVertexShader,
                           std::string fileFragmentShader);
  void GenerateTextures() override;
  void BindAllTextures() override;

  unsigned int env_cube_map_texture();

 private:
  unsigned int env_cube_map_texture_;
};

#endif