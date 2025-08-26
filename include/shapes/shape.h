#ifndef SHAPES_SHAPE_H
#define SHAPES_SHAPE_H

class Shape {
 public:
  virtual void Draw() = 0;

 protected:
  virtual void Init() = 0;
};

#endif