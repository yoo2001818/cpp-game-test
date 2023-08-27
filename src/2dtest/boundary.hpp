#ifndef BOUNDARY_HPP_
#define BOUNDARY_HPP_

#include "rect.hpp"
#include "transform.hpp"

class boundary {
  enum shape {
    circle,
    rectangle,
    capsule
  };
  public:
  ::rect rect;
  shape shape;

  ::rect getWorldRect(const transform& transform);
};

#endif // BOUNDARY_HPP_

