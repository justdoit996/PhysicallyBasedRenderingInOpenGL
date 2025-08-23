#include "shapes/sphere.h"

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include <cmath>
#include <glm/glm.hpp>

Sphere::Sphere(int sectors, int stacks) : sectors_(sectors), stacks_(stacks) {
  Init();
}

void Sphere::Init() {
  glGenVertexArrays(1, &VAO_);

  unsigned int VBO, EBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  std::vector<glm::vec3> vertex_positions;
  std::vector<glm::vec2> uv_coordinates;
  std::vector<glm::vec3> triangle_normals;
  std::vector<unsigned int> indices;
  const float PI = std::acos(-1);

  for (int x = 0; x < sectors_; x++) {
    for (int y = 0; y < stacks_; y++) {
      // uv_coordinates mapping
      float u = 1.0 * x / sectors_;
      float v = 1.0 * y / stacks_;
      // Spherical coords
      float xPos = std::cos(u * 2.0f * PI) * std::sin(v * PI);
      float yPos = std::cos(v * PI);
      float zPos = std::sin(u * 2.0f * PI) * std::sin(v * PI);

      vertex_positions.emplace_back(glm::vec3(xPos, yPos, zPos));
      uv_coordinates.emplace_back(glm::vec2(u, v));
      // Not actual triangle_normals. Will be computed later.
      triangle_normals.emplace_back(glm::vec3(xPos, yPos, zPos));
    }
  }

  // generate CCW index list of sphere triangles
  // k1--k1+1
  // |  / |
  // | /  |
  // k2--k2+1
  int k1, k2;
  for (int y = 0; y < stacks_; y++) {
    k1 = y * (sectors_ + 1);
    k2 = k1 + sectors_ + 1;
    for (int x = 0; x < sectors_; x++, k1++, k2++) {
      if (y != 0) {
        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k1 + 1);
      }
      if (y != stacks_ - 1) {
        indices.push_back(k1 + 1);
        indices.push_back(k2);
        indices.push_back(k2 + 1);
      }
    }
  }
  index_count_ = indices.size();

  std::vector<float> data;
  for (unsigned int i = 0; i < vertex_positions.size(); ++i) {
    data.push_back(vertex_positions[i].x);
    data.push_back(vertex_positions[i].y);
    data.push_back(vertex_positions[i].z);
    if (triangle_normals.size() > 0) {
      data.push_back(triangle_normals[i].x);
      data.push_back(triangle_normals[i].y);
      data.push_back(triangle_normals[i].z);
    }
    if (uv_coordinates.size() > 0) {
      data.push_back(uv_coordinates[i].x);
      data.push_back(uv_coordinates[i].y);
    }
  }
  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0],
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);
  unsigned int stride = (3 + 2 + 3) * sizeof(float);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                        (void*)(6 * sizeof(float)));
}

void Sphere::BindAndDraw() {
  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLE_STRIP, index_count_, GL_UNSIGNED_INT, 0);
}