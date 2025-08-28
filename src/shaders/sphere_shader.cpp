#include "shaders/sphere_shader.h"

#include <glm/glm.hpp>
#include "utils/utility.h"

SphereShader::SphereShader() {}

SphereShader::SphereShader(std::string fileVertexShader,
                           std::string fileFragmentShader)
    : Shader(fileVertexShader, fileFragmentShader) {}

void SphereShader::LoadTextures(const std::string& path) {
  // Load textures
  albedo_map_ = LoadTexture(path + "/albedo.png");
  normal_map_ = LoadTexture(path + "/normal.png");
  metallic_map_ = LoadTexture(path + "/metallic.png");
  roughness_map_ = LoadTexture(path + "/roughness.png");
  ao_map_ = LoadTexture(path + "/ao.png");

  // Texture uniforms
  this->Use();
  this->SetInt("albedoMap", 0);
  this->SetInt("normalMap", 1);
  this->SetInt("metallicMap", 2);
  this->SetInt("roughnessMap", 3);
  this->SetInt("aoMap", 4);
}

void SphereShader::BindAllTextures() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, albedo_map_);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, normal_map_);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, metallic_map_);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, roughness_map_);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, ao_map_);
}