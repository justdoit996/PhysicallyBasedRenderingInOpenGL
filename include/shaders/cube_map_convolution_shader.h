#ifndef CUBE_MAP_CONVOLUTION_SHADER_H
#define CUBE_MAP_CONVOLUTION_SHADER_H

#include "shaders/shader.h"

// Pre-computes the irradiance of a cube map using convolution
class CubeMapConvolutionShader : public Shader {
 public:
  CubeMapConvolutionShader(std::string fileVertexShader,
                           std::string fileFragmentShader);

  void GenerateTextures() override;
  void BindAllTextures() override;

 private:
  unsigned int irradiance_map_texture_;
};

#endif
