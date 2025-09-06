#ifndef SHAPES_QUAD_H_
#define SHAPES_QUAD_H_

#define GLAD_GL_IMPLEMENTATION
#include "glad.h"

#include "shapes/shape.h"

class Quad : public Shape {
 public:
  Quad();
  void Draw() override;

 protected:
  void Init() override;

 private:
  unsigned int VAO_;
};

#endif