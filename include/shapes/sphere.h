#ifndef SHAPES_SPHERE_H
#define SHAPES_SPHERE_H

#include <vector>

class Sphere {
 public:
  Sphere(int sectors, int stacks);
  void Draw();

 private:
  void Init();

  int sectors_;
  int stacks_;

  unsigned int VAO_ = 0;
  // Number of indices in the sphere. Counts more than the total vertices due
  // to triangles sharing indices.
  unsigned int index_count_;
};

#endif