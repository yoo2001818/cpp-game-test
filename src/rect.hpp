#ifndef RECT_HPP_
#define RECT_HPP_

#include <optional>
#include <glm/glm.hpp>

class rect {
  public:
  alignas(16) glm::vec3 min {0., 0., 0.};
  alignas(16) glm::vec3 max {1., 1., 1.};
  
  std::optional<rect> intersectRect(rect target);
  rect unionRect(rect target);
};

#endif // RECT_HPP_
