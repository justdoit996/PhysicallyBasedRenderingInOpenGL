#ifndef SKYBOX_SHADER_H
#define SKYBOX_SHADER_H

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include "shaders/shader.h"

class CubeMapShader : public Shader {
 public:
  CubeMapShader(std::string fileVertexShader, std::string fileFragmentShader);
  CubeMapShader() = default;
  void LoadTextures(const std::string& path = "") override;
  void BindAllTextures() override;

 private:
  unsigned int env_cube_map_texture_;
};

#endif