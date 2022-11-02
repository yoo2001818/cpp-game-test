#ifndef PHYSICS_HPP_
#define PHYSICS_HPP_

#include <glm/glm.hpp>
#include <vector>

#include "game.hpp"
#include "entity.hpp"

namespace physics {
  struct collision2 {
    entity_id target1;
    entity_id target2;
    glm::vec3 direction;
    glm::vec3 position;
  };

  struct collision {
    entity_id target;
    glm::vec3 direction;
    glm::vec3 position;
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
