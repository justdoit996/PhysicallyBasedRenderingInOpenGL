#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/glm.hpp>

class PointLight {
 public:
  PointLight() = default;
  PointLight(const glm::vec3& position,
             const glm::vec3& color,
             const float& i = 1.f);
  glm::vec3 GetColor() const;
  // Returns the color amplified by the intensity
  glm::vec3 GetTotalColor() const;

  glm::vec3 position;
  glm::vec3 color;
  float intensity;
};

#endif