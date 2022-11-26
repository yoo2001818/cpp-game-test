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

  // |  A  |
  //       |  B  |
  // 
  //       + <-> -
  auto x_max = a_bounds.max.x - b_bounds.min.x;

  //       |  A  |
  // |  B  |
  // 
  // - <-> +
  auto x_min = b_bounds.max.x - a_bounds.min.x;

  // Horizontal collision
  // (We require some tolerance to avoid horizontal collision to wrongly occur)
  const auto TOLERANCE_X = 0.3;
  if (y_max >= TOLERANCE_X && y_min >= TOLERANCE_X) {
    if (x_max >= 0.0 && x_min > x_max) {
      return glm::vec3(-x_max, 0.0, 0.0);
    } else if (x_min >= 0.0 && x_max > x_min) {
      return glm::vec3(x_min, 0.0, 0.0);
    }
  }

  // Vertical collision
  const auto TOLERANCE_Y = 0.05;
  if (x_max >= TOLERANCE_Y && x_min >= TOLERANCE_Y) {
    if (y_max >= 0.0 && y_min > y_max) {
      // Counteract A to move up (Y-)
      return glm::vec3(0.0, -y_max, 0.0);
    } else if (y_min >= 0.0 && y_max > y_min) {
      // Counteract A to move down (Y+)
      return glm::vec3(0.0, y_min, 0.0);
    }
  }

  // Nothing collided
  return std::nullopt;
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
    physics_val.force += glm::vec3(0., 18.0 * physics_val.mass * step_size, 0.);
    // Air resistance
    physics_val.force -=
      glm::sign(physics_val.velocity) *
      physics_val.velocity * physics_val.velocity * surface_size * glm::vec3(0.5 * 0.8, 0.5 * 0.3, 1.0) * step_size;
  }

  // Collision check phase
  for (auto entity : query) {
    auto [physics_val, transform_val, boundary_val] = entity->get<physics, transform, boundary>();

    // Collision check
    auto a_rect = boundary_val.getWorldRect(transform_val);
    tile_index::tile min {
      static_cast<int32_t>(std::floor(a_rect.min.x)),
      static_cast<int32_t>(std::floor(a_rect.min.y)),
    };
    tile_index::tile max {
      static_cast<int32_t>(std::ceil(a_rect.max.x)),
      static_cast<int32_t>(std::ceil(a_rect.max.y)),
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
          auto b_rect = target_boundary.getWorldRect(target_transform);

          // Run collision test
          auto a_normal = intersectRectRect(a_rect, b_rect);
          if (!a_normal.has_value()) continue;

          glm::vec3 ab_velocity;
          if (target_physics != nullptr) {
            ab_velocity = physics_val.velocity - target_physics->velocity;
          } else {
            ab_velocity = physics_val.velocity;
          }

          auto vrn = glm::dot(ab_velocity, a_normal.value());

          if (vrn > 0.0) continue;

          collisions.push_back({
            target1: entity->getId(),
            target2: target_id,
            normal: a_normal.value(),
          });
        }
      }
    }
  }
  // Impulse generation phase
  for (auto collision : collisions) {
    auto a_entity = game.mWorld.get(collision.target1);
    auto b_entity = game.mWorld.get(collision.target2);

    if (a_entity == nullptr || b_entity == nullptr) {
      // This can't happen
      continue;
    }

    auto [a_physics, a_transform, a_boundary] = a_entity->get<physics, transform, boundary>();
    auto [b_transform, b_boundary] = b_entity->get<transform, boundary>();
    auto b_physics = b_entity->try_get<physics>();

    glm::vec3 ab_velocity;
    if (b_physics != nullptr) {
      ab_velocity = a_physics.velocity - b_physics->velocity;
    } else {
      ab_velocity = a_physics.velocity;
    }

    // Cancel movement
    // TODO: Get ratio between A and B, and move them accordingly
    a_transform.position += collision.normal;

    auto normal_dir = glm::normalize(collision.normal);
    
    // Generate impulse energy
    auto vrn = glm::dot(ab_velocity, normal_dir);
    float impact_energy = -vrn * (0.8) * a_physics.mass;
    auto fi = normal_dir * impact_energy;
    a_physics.force += fi;
    a_physics.onGround = -10;

    // Update collision grid right away
    game.mWorld.markDirty(*a_entity);
    game.mWorld.updateIndex();
  }
}
