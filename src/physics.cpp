#include <iostream>

#include "physics.hpp"
#include "transform.hpp"
#include "boundary.hpp"

void physics::updatePhysics(game& game) {
  auto query = game.mWorld.getQuery<physics, transform, boundary>();
  for (auto entity : query) {
    auto [physics_val, transform_val, boundary_val] = entity->get<physics, transform, boundary>();

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
          auto target_rect = target_boundary.getWorldRect(target_transform);
          auto pos_diff = target_transform.position - prev_position;
          // Run collision check...
          auto intersection = world_rect.intersectRect(target_rect);
          if (!intersection.has_value()) continue;
          auto intersection_rect = intersection.value();
          // Move to the opposite direction to avoid collision
          auto intersection_size = intersection_rect.max - intersection_rect.min;
          if (intersection_size.y < intersection_size.x) {
            transform_val.position.y -= glm::sign(pos_diff.y) * intersection_size.y;
            if (glm::abs(prev_velocity.y) < 0.01) {
              physics_val.velocity.y = 0.0;
            } else {
              physics_val.velocity.y = -prev_velocity.y * 0.1;
            }
            // physics_val.velocity.x /= 1.0 + intersection_size.x * 0.2;
          } else {
            transform_val.position.x -= glm::sign(pos_diff.x) * intersection_size.x;
            if (glm::abs(prev_velocity.x) < 0.01) {
              physics_val.velocity.x = 0.0;
            } else {
              physics_val.velocity.x = -prev_velocity.x * 0.1;
            }
            // physics_val.velocity.y /= 1.0 + intersection_size.y * 0.2;
          }
          // TODO: Handle physics-physics object collision
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
