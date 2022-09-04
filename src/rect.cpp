#include "rect.hpp"

std::optional<rect> rect::intersectRect(rect target) {
  rect result {
    glm::max(this->min, target.min),
    glm::min(this->max, target.max),
  };

  if (
    result.max.x <= result.min.x ||
    result.max.y <= result.min.y ||
    result.max.z <= result.min.z
  ) {
    return std::nullopt;
  }
  return result;
}

rect rect::unionRect(rect target) {
  return {
    glm::min(this->min, target.min),
    glm::max(this->max, target.max),
  };
}
