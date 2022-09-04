#ifndef BOUNDARY_HPP_
#define BOUNDARY_HPP_

#include "rect.hpp"
#include "transform.hpp"

class boundary {
  public:
  ::rect rect;

  ::rect getWorldRect(const transform& transform);
};

#endif // BOUNDARY_HPP_

