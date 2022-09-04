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
    physics_val.force -= physics_val.velocity * physics_val.velocity * surface_size * glm::vec3(0.5 * 0.2);

    // Apply net force to velocity & update position
    physics_val.velocity += physics_val.force / physics_val.mass;
    transform_val.position += physics_val.velocity;

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
          auto target_rect = target_boundary.getWorldRect(target_transform);
          // Run collision check...
          auto intersection = world_rect.intersectRect(target_rect);
          if (!intersection.has_value()) continue;
          auto intersection_rect = intersection.value();
          // Move to the opposite direction to avoid collision
          auto intersection_size = intersection_rect.max - intersection_rect.min;
          float move_amount;
          auto abs_velocity = glm::abs(physics_val.velocity);
          if (abs_velocity.x > abs_velocity.y) {
            move_amount = intersection_size.x / abs_velocity.x;
          } else {
            move_amount = intersection_size.y / abs_velocity.y;
          }
          transform_val.position -= physics_val.velocity * move_amount;
          // TODO: Handle physics-physics object collision
          // Reset velocity
          physics_val.velocity = glm::vec3(0.0);
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
