#ifndef PHYSICS_HPP_
#define PHYSICS_HPP_

#include <glm/glm.hpp>
#include <vector>

#include "game.hpp"
#include "entity.hpp"

namespace physics {
  struct collision {
    entity_id target;
    glm::vec3 direction;
  };

  struct physics {
    alignas(16) glm::vec3 velocity {0., 0., 0.};
    alignas(16) glm::vec3 force {0., 0., 0.};
    float mass = 1;
    int onGround = 0;
    bool hasCollision = false;
    bool hasCollisionHandler = false;
    std::vector<collision> collisions;
  };

  void updatePhysics(game& game);
};
#endif
