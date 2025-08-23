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
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> uv_coordinates;
  std::vector<unsigned int> indices;
  const float PI = std::acos(-1);

  for (int x = 0; x <= sectors_; x++) {
    for (int y = 0; y <= stacks_; y++) {
      // uv_coordinates mapping
      float u = 1.0 * x / sectors_;
      float v = 1.0 * y / stacks_;
      // Spherical coords
      float xPos = std::cos(u * 2.0f * PI) * std::sin(v * PI);
      float yPos = std::cos(v * PI);
      float zPos = std::sin(u * 2.0f * PI) * std::sin(v * PI);

      vertex_positions.emplace_back(glm::vec3(xPos, yPos, zPos));
      normals.emplace_back(glm::vec3(xPos, yPos, zPos));
      uv_coordinates.emplace_back(glm::vec2(u, v));
    }
  }

  // generate CCW index list of sphere triangles
  // k1--k1+1
  // |  / |
  // | /  |
  // k2--k2+1
  int k1, k2;
  for (int i = 0; i < stacks_; ++i) {
    k1 = i * (sectors_ + 1);  // beginning of current stack
    k2 = k1 + sectors_ + 1;   // beginning of next stack

    for (int j = 0; j < sectors_; ++j, ++k1, ++k2) {
      // 2 triangles per sector excluding first and last stacks
      // k1 => k2 => k1+1
      if (i != 0) {
        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k1 + 1);
      }

      // k1+1 => k2 => k2+1
      if (i != (stacks_ - 1)) {
        indices.push_back(k1 + 1);
        indices.push_back(k2);
        indices.push_back(k2 + 1);
      }
    }
  }
  index_count_ = indices.size();

  // Put vertex, normals, and texture mappings into a single array
  std::vector<float> data;
  for (unsigned int i = 0; i < vertex_positions.size(); ++i) {
    data.push_back(vertex_positions[i].x);
    data.push_back(vertex_positions[i].y);
    data.push_back(vertex_positions[i].z);
    if (normals.size() > 0) {
      data.push_back(normals[i].x);
      data.push_back(normals[i].y);
      data.push_back(normals[i].z);
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
  unsigned int stride = (3 + 3 + 2) * sizeof(float);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                        (void*)(6 * sizeof(float)));
  glBindVertexArray(0);
}

void Sphere::BindAndDraw() {
  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLE_STRIP, index_count_, GL_UNSIGNED_INT, 0);
}