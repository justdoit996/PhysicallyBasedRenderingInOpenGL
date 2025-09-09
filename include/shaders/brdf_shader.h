#ifndef BRDF_SHADER_H
#define BRDF_SHADER_H

#include <string>
#include "shaders/shader.h"

class BrdfShader : public Shader {
 public:
  BrdfShader() = default;
  BrdfShader(std::string fileVertexShader, std::string fileFragmentShader);
  void GenerateTextures() override;
  void BindAllTextures() override;
  unsigned int brdf_lut_texture() { return brdf_lut_texture_; }

 private:
  unsigned int brdf_lut_texture_;
};

#endif