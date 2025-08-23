#ifndef SHAPES_SPHERE_H
#define SHAPES_SPHERE_H

#include <vector>

class Sphere {
 public:
  Sphere(int sectors, int stacks);

 private:
  int sectors_;
  int stacks_;
};

#endif