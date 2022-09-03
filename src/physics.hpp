#ifndef PHYSICS_HPP_
#define PHYSICS_HPP_

#include <glm/glm.hpp>

#include "game.hpp"

namespace physics {
  class physics {
    alignas(16) glm::vec3 value {0., 0., 0.};
  };

  void updatePhysics(game& game);
};
#endif
