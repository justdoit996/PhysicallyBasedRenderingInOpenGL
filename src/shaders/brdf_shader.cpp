#include "shaders/brdf_shader.h"

BrdfShader::BrdfShader(std::string fileVertexShader,
                       std::string fileFragmentShader)
    : Shader(fileVertexShader, fileFragmentShader) {}

void BrdfShader::GenerateTextures() {}

void BrdfShader::BindAllTextures() {}