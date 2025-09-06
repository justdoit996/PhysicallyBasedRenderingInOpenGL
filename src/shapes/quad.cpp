#include "shapes/quad.h"

#include <glm/glm.hpp>
#include "utils/constants.h"

Quad::Quad() {
  Init();
}

void Quad::Init() {
  unsigned int quad_VBO;
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &quad_VBO);
  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(constants::vertical_quad_vertices),
               &constants::vertical_quad_vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
}

void Quad::Draw() {
  glBindVertexArray(VAO_);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}