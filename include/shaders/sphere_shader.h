#ifndef SPHERE_SHADER_H
#define SPHERE_SHADER_H

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include "shaders/shader.h"

#include <string>

class SphereShader : public Shader {
 public:
  SphereShader();
  SphereShader(std::string fileVertexShader, std::string fileFragmentShader);
  SphereShader& operator=(const SphereShader&) = default;

  void LoadTextures(const std::string& path) override;
  void BindAllTextures() override;
  void BindTexture(const std::string& uniform, unsigned int texture_id);

 private:
  unsigned int albedo_map_;
  unsigned int normal_map_;
  unsigned int metallic_map_;
  unsigned int roughness_map_;
  unsigned int ao_map_;
};

#endif