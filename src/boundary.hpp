#ifndef BOUNDARY_HPP_
#define BOUNDARY_HPP_

#include <glm/glm.hpp>

struct boundary {
  alignas(16) glm::vec3 min {0., 0., 0.};
  alignas(16) glm::vec3 max {1., 1., 1.};
};

#endif // BOUNDARY_HPP_

