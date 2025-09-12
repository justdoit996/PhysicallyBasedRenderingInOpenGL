#include "point_light.h"

PointLight::PointLight(const glm::vec3& p, const glm::vec3& c)
    : position(p), color(c) {}

PointLight::PointLight(glm::vec3&& p, glm::vec3&& c)
    : position(std::move(p)), color(std::move(c)) {}

glm::vec3 PointLight::GetColor() const {
  return color;
}

glm::vec3 PointLight::GetColorLuminance() const {
  return color * intensity;
}
