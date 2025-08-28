#ifndef IRRADIANCE_CUBE_MAP_SHADER_H
#define IRRADIANCE_CUBE_MAP_SHADER_H

#include "shaders/shader.h"

// Pre-computes the irradiance of a cube map using convolution
class IrradianceCubeMapShader : public Shader {
 public:
  IrradianceCubeMapShader() = default;
  IrradianceCubeMapShader(std::string fileVertexShader,
                          std::string fileFragmentShader);

  void GenerateTextures() override;
  void BindAllTextures() override;

  unsigned int irradiance_map_texture();

 private:
  unsigned int irradiance_map_texture_;
};

#endif
