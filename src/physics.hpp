#ifndef PHYSICS_HPP_
#define PHYSICS_HPP_

#include <glm/glm.hpp>
#include <vector>

#include "game.hpp"

namespace physics {
  struct collision {};

  struct physics {
    alignas(16) glm::vec3 velocity {0., 0., 0.};
    alignas(16) glm::vec3 force {0., 0., 0.};
    float mass = 1;
    int onGround = 0;
    bool hasCollisionHandler = false;
    std::vector<collision> collisions;
  };

  void updatePhysics(game& game);
};
#endif
