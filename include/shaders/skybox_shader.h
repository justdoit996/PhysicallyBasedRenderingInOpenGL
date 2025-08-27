#ifndef SKYBOX_SHADER_H
#define SKYBOX_SHADER_H

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include "shaders/shader.h"

class SkyboxShader : public Shader {
 public:
  SkyboxShader(std::string fileVertexShader, std::string fileFragmentShader);
  SkyboxShader() = default;

 private:
  void LoadTextures(const std::string& path) override;
  void BindAllTextures() override;

  unsigned int env_cube_map_texture_;
};

#endif