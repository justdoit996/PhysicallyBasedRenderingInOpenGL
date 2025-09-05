#ifndef PREFILTER_SHADER_H_
#define PREFILTER_SHADER_H_

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include "shaders/shader.h"

class PrefilterShader : public Shader {
 public:
  PrefilterShader() = default;
  PrefilterShader(std::string fileVertexShader, std::string fileFragmentShader);

  void GenerateTextures() override;
  void BindAllTextures() override;

  unsigned int prefilter_map_texture();

 private:
  unsigned int prefilter_map_texture_;
};

#endif