#ifndef PHYSICS_HPP_
#define PHYSICS_HPP_

#include <glm/glm.hpp>

#include "game.hpp"

namespace physics {
  struct physics {
    alignas(16) glm::vec3 velocity {0., 0., 0.};
    alignas(16) glm::vec3 force {0., 0., 0.};
    float mass = 1;
    bool onGround;
  };

  void updatePhysics(game& game);
};
#endif
