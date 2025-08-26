#ifndef SHAPES_CUBE_H
#define SHAPES_CUBE_H

#include "shapes/shape.h"

class Cube : public Shape {
 public:
  Cube();
  void Draw() override;

 private:
  unsigned int VAO_;
  void Init() override;
};

#endif