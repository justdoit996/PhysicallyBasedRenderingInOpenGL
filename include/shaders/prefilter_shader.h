#ifndef PREFILTER_SHADER_H_
#define PREFILTER_SHADER_H_

#include "shaders/shader.h"

class PrefilterShader : public Shader {
 public:
  PrefilterShader() = default;
  PrefilterShader(std::string fileVertexShader, std::string fileFragmentShader);
};

#endif