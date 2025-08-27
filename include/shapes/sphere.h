#ifndef SHAPES_SPHERE_H
#define SHAPES_SPHERE_H

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include "shapes/shape.h"

#include <vector>

class Sphere : public Shape {
 public:
  Sphere(int sectors, int stacks);
  void Draw() override;

 private:
  void Init() override;

  int sectors_;
  int stacks_;

  unsigned int VAO_ = 0;
  // Number of indices in the sphere. Counts more than the total vertices due
  // to triangles sharing indices.
  unsigned int index_count_;
};

#endif