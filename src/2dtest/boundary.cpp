#include "boundary.hpp"

::rect boundary::getWorldRect(const transform& transform) {
  return {
    transform.position + rect.min,
    transform.position + rect.max
  };
}
