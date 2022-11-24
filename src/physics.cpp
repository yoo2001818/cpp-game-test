#include <iostream>

#include "physics.hpp"
#include "transform.hpp"
#include "boundary.hpp"

std::optional<glm::vec3> intersectRectRect(
  rect a_bounds,
  rect b_bounds
) {
  // - Vertical collision takes precedence
  // - We assume that only A moves (B is treated as a rectangle, A is treated as a line)

  //      A        +
  // ---+---+----
  //    | B |  ^   -
  //    +---+  |
  auto y_max = a_bounds.max.y - b_bounds.min.y;

  //    +---+  |
  //    | B |  v   -
  // ---+---+----
  //      A        +
  auto y_min = b_bounds.max.y - a_bounds.min.y;
}

void physics::updatePhysics(game& game) {
  auto query = game.mWorld.getQuery<physics, transform, boundary>();
  std::vector<collision2> collisions;

  // The collision process routine is as follows:
  //
  // 1. Update force, velocity, position
  // 2. Check for collision, and mark the following for each collision:
  //    - Entity # (A, B)
  //    - Normal (A -> B)
  //    - Penetration vector (A -> B)
  //    - Impact energy
  // 3. For each collision, move entities around and apply impact force
  //    - Unwind penetration
  //    - Apply impact energy

  float step_size = 1.0 / 60.0;

  // Movement phase
  for (auto entity : query) {
    auto [physics_val, transform_val, boundary_val] = entity->get<physics, transform, boundary>();

    physics_val.velocity += physics_val.force / physics_val.mass;
    transform_val.position += physics_val.velocity * step_size;
    physics_val.onGround += 1;

    // Reset previous force
    physics_val.force = glm::vec3(0.0);
    
    auto surface_size = glm::abs(boundary_val.rect.max - boundary_val.rect.min);
    // Gravity
    physics_val.force += glm::vec3(0., 0.3 * physics_val.mass * step_size, 0.);
    // Air resistance
    physics_val.force -=
      glm::sign(physics_val.velocity) *
      physics_val.velocity * physics_val.velocity * surface_size * glm::vec3(0.5 * 0.8, 0.5 * 0.3, 1.0) * step_size;
  }

  // Collision check phase
  for (auto entity : query) {
    auto [physics_val, transform_val, boundary_val] = entity->get<physics, transform, boundary>();

    // Collision check
    auto world_rect = boundary_val.getWorldRect(transform_val);
    tile_index::tile min {
      static_cast<int32_t>(std::floor(world_rect.min.x)),
      static_cast<int32_t>(std::floor(world_rect.min.y)),
    };
    tile_index::tile max {
      static_cast<int32_t>(std::ceil(world_rect.max.x)),
      static_cast<int32_t>(std::ceil(world_rect.max.y)),
    };
    for (int y = min[1]; y < max[1]; y += 1) {
      for (int x = min[0]; x < max[0]; x += 1) {
        tile_index::tile current {x, y};
        auto targets = game.mWorld.getTileIndex().get(current);
        for (auto target_id : targets) {
          // Skip itself for obvious reasons
          if (target_id == entity->getId()) continue;
          auto target = game.mWorld.get(target_id);
          auto [target_transform, target_boundary] = target->get<transform, boundary>();
          auto target_physics = target->try_get<physics>();
          auto target_rect = target_boundary.getWorldRect(target_transform);
          auto pos_diff = target_transform.position - transform_val.position;
          glm::vec3 velocity_diff;
          if (target_physics != nullptr) {
            velocity_diff = physics_val.velocity - target_physics->velocity;
          } else {
            velocity_diff = physics_val.velocity;
          }
          /*
          // Run collision check...
          auto intersection = world_rect.intersectRect(target_rect);
          if (!intersection.has_value()) continue;
          auto intersection_rect = intersection.value();
          // Determine the collided edge
          
          // This is extremely tricky because without enough information, it is
          // easy to wrongly determine the collided side.
          // For example, if the object collided at the corner from the top,
          // it could be detected as it collided with the right/left. Instead
          // of pushing the object to top, it will push the object to the side,
          // making the object move abruptly to other direction.

          // To prevent this behavior, we need to put the object velocity
          // (or the difference between the current position and the previous
          // position... which is velocity.) in account.

          // Instead of using collision box size to determine the collided
          // position, we can solely depend on the velocity to determine the
          // collided edge.

          //  |     |
          //  |  1  |   --->
          //  |     |   |     |
          //            |  2  |
          //            |     |

          // If 1 and 2 collides, the normal will be LEFT from 1's view,
          // and RIGHT from 2's view. (Because each object should move along 
          // that direction)

          // However this is also flawed as well, as it will report wrong
          // direction when the object is sliding along the edge on other
          // object. ... We need BOTH logic to determine the position!

          // Using the velocity to determine the previous position, it basically
          // boils to this problem: While continously moving the object from
          // the previous position to current position, which edge gets collided
          // first?

          // The answer can be simply derived - If we were to cancel the
          // each object's movement to the exact point where they collide -
          // which edge would result in smaller movement?

          // ... It doesn't work. We need robust tests to handle all AABB
          // collision case to determine correct collision vectors.
          
          auto intersection_size = intersection_rect.max - intersection_rect.min;
          auto move_diff_ratio = intersection_size / physics_val.velocity;

          glm::vec3 normal;
          glm::vec3 move_diff;
          auto move_diff_abs = glm::abs(move_diff_ratio);
          // Compare midpoint
          auto target_mid = (target_rect.min + target_rect.max) / 2.0f;
          auto world_mid = (world_rect.min + world_rect.max) / 2.0f;
          if (move_diff_abs.x < move_diff_abs.y) {
            move_diff = physics_val.velocity * move_diff_ratio.x;
            if (target_mid.x < world_mid.x) {
              // (target is on the) left
              normal = glm::vec3(1.0, 0.0, 0.0);
            } else {
              // right
              normal = glm::vec3(-1.0, 0.0, 0.0);
            }
          } else {
            move_diff = physics_val.velocity * move_diff_ratio.y;
            if (target_mid.y < world_mid.y) {
              // (target is on the) bottom (note that we didn't flip axis yet)
              normal = glm::vec3(0.0, 1.0, 0.0);
            } else {
              // (target is on the) top
              normal = glm::vec3(0.0, -1.0, 0.0);
              physics_val.onGround = -10;
            }
          }
          // Check if the collision should continue (is the entities colliding?)
          if (glm::dot(glm::normalize(velocity_diff), normal) >= 0.0) {
            continue;
          }
          // Move to the opposite direction to avoid collision
          if (glm::length(move_diff) > 0.5f) {
            std::cout << "Object moved too much; " << move_diff.x << ", " << move_diff.y << std::endl;
            std::cout << "velocity: " << physics_val.velocity.x << ", " << physics_val.velocity.y << std::endl;
            std::cout << "intersection: " << intersection_size.x << ", " << intersection_size.y << std::endl;
          }
          transform_val.position -= move_diff;
          world_rect = boundary_val.getWorldRect(transform_val);
          // Calculate impact energy
          auto vrn = glm::dot(velocity_diff, normal);
          float impact_energy = -vrn * (1.5) * physics_val.mass;
          auto fi = normal * impact_energy;
          physics_val.force += fi;
          // TODO: Handle physics-physics object collision
          auto intersection_mid = (intersection_rect.min + intersection_rect.max) / 2.0f;
          if (physics_val.hasCollisionHandler) {
            physics_val.collisions.push_back({ target_id, normal, intersection_mid });
          }
          physics_val.hasCollision = true;
          if (target->has<physics>()) {
            auto target_physics = target->get<physics>();
            if (target_physics.hasCollisionHandler) {
              target_physics.collisions.push_back({ entity->getId(), -normal, intersection_mid });
            }
            target_physics.hasCollision = true;
          }
          */
        }
      }
    }

    // Collision update phase
    for (auto collision : collisions) {
      // Calculate impulse, update velocity/position, etc
    }

    // Update collision grid right away
    game.mWorld.markDirty(*entity);
    game.mWorld.updateIndex();
  }
}
