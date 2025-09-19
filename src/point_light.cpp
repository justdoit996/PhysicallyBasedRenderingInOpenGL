#include "point_light.h"

PointLight::PointLight(const glm::vec3& p, const glm::vec3& c, const float& i)
    : position(p), color(c), intensity(i) {}

glm::vec3 PointLight::GetColor() const {
  return color;
}

glm::vec3 PointLight::GetTotalColor() const {
  return color * intensity;
}
