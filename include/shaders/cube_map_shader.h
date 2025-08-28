#ifndef SKYBOX_SHADER_H
#define SKYBOX_SHADER_H

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include "shaders/shader.h"

class CubeMapShader : public Shader {
 public:
  CubeMapShader(std::string fileVertexShader, std::string fileFragmentShader);
  CubeMapShader() = default;
  void GenerateTextures() override;
  void BindAllTextures() override;

  unsigned int env_cube_map_texture();

 private:
  unsigned int env_cube_map_texture_;
};

#endif