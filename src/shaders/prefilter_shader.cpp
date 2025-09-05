#include "shaders/prefilter_shader.h"

PrefilterShader::PrefilterShader(std::string fileVertexShader,
                                 std::string fileFragmentShader)
    : Shader(fileVertexShader, fileFragmentShader) {}