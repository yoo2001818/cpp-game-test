#include <iostream>

#include "physics.hpp"
#include "transform.hpp"
#include "boundary.hpp"

void physics::updatePhysics(game& game) {
  auto query = game.mWorld.getQuery<physics, transform, boundary>();
  for (auto entity : query) {
    auto [physics_val, transform_val, boundary_val] = entity->get<physics, transform, boundary>();

    if (physics_val.hasCollisionHandler) {
      physics_val.collisions.clear();
    }

    auto surface_size = glm::abs(boundary_val.rect.max - boundary_val.rect.min);
    // Gravity
    physics_val.force += glm::vec3(0., 0.3 / 60.0 * physics_val.mass, 0.);
    // Air resistance
    physics_val.force -=
      glm::sign(physics_val.velocity) *
      physics_val.velocity * physics_val.velocity * surface_size * glm::vec3(0.5 * 0.8, 0.5 * 0.3, 1.0);

    // Apply net force to velocity & update position
    physics_val.velocity += physics_val.force / physics_val.mass;
    transform_val.position += physics_val.velocity;
    physics_val.onGround += 1;

    // Collision check
    auto prev_velocity = physics_val.velocity;
    auto prev_position = transform_val.position;
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
          auto pos_diff = target_transform.position - prev_position;
          glm::vec3 velocity_diff;
          if (target_physics != nullptr) {
            velocity_diff = physics_val.velocity - target_physics->velocity;
          } else {
            velocity_diff = physics_val.velocity;
          }
          // Run collision check...
          auto intersection = world_rect.intersectRect(target_rect);
          if (!intersection.has_value()) continue;
          auto intersection_rect = intersection.value();
          // Determine the collided edge
          glm::vec3 normal;
          auto intersection_size = intersection_rect.max - intersection_rect.min;
          if (intersection_size.x < intersection_size.y) {
            if (target_rect.min.x < world_rect.min.x) {
              // (target is on the) left
              normal = glm::vec3(1.0, 0.0, 0.0);
            } else {
              // right
              normal = glm::vec3(-1.0, 0.0, 0.0);
            }
          } else {
            if (target_rect.min.y < world_rect.min.y) {
              // (target is on the) bottom (note that we didn't flip axis yet)
              normal = glm::vec3(0.0, 1.0, 0.0);
            } else {
              // (target is on the) top
              normal = glm::vec3(0.0, -1.0, 0.0);
              physics_val.onGround = -10;
            }
          }
          // Check if the collision should continue (is the entities colliding?)
          if (glm::dot(velocity_diff, normal) >= 0.0) {
            continue;
          }
          // Move to the opposite direction to avoid collision
          if (intersection_size.y < intersection_size.x) {
            transform_val.position -= physics_val.velocity * (intersection_size.y / physics_val.velocity.y);
          } else {
            transform_val.position -= physics_val.velocity * (intersection_size.x / physics_val.velocity.x);
          }
          // Calculate impact energy
          auto vrn = glm::dot(physics_val.velocity, normal);
          float impact_energy = -vrn * (1.5) * physics_val.mass;
          auto fi = normal * impact_energy;
          physics_val.velocity += fi / physics_val.mass;
          // TODO: Handle physics-physics object collision
          if (physics_val.hasCollisionHandler) {
            physics_val.collisions.push_back({ target_id, pos_diff });
          }
          if (target->has<physics>()) {
            auto target_physics = target->get<physics>();
            if (target_physics.hasCollisionHandler) {
              target_physics.collisions.push_back({ entity->getId(), -pos_diff });
            }
          }
        }
      }
    }

    // Reset force for next loop
    physics_val.force = glm::vec3(0.0);

    // Update collision grid right away
    game.mWorld.markDirty(*entity);
    game.mWorld.updateIndex();
  }
}
