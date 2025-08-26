#include "shapes/cube.h"

#include <glm/glm.hpp>
#include "glad.h"
#include "utils/constants.h"

Cube::Cube() {
  Init();
}

void Cube::Init() {
  unsigned int cube_VBO;
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &cube_VBO);
  // fill buffer
  glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(constants::cube_vertices_and_normals_and_texture_coords),
               constants::cube_vertices_and_normals_and_texture_coords,
               GL_STATIC_DRAW);
  // link vertex attributes
  glBindVertexArray(VAO_);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(6 * sizeof(float)));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Cube::Draw() {
  glBindVertexArray(VAO_);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}
