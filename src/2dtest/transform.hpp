#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct transform {
  alignas(16) glm::vec3 position {0., 0., 0.};
  alignas(16) glm::vec3 scale {1., 1., 1.};
  alignas(16) glm::quat rotation {0., 0., 0., 1.};
};

#endif // TRANSFORM_HPP_
