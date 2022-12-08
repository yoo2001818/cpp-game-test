#include <iostream>
#include <algorithm>
#include <cmath>

#include "physics.hpp"
#include "transform.hpp"
#include "boundary.hpp"

struct collision_result {
  glm::vec3 normal;
  float length;
};

std::optional<collision_result> intersectRectRect(
    rect a_bounds,
    rect b_bounds)
{
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
  if (y_max >= TOLERANCE_X && y_min >= TOLERANCE_X)
  {
    if (x_max >= 0.0 && x_min > x_max)
    {
      collision_result result { glm::vec3(-1.0, 0.0, 0.0), x_max };
      return result;
    }
    else if (x_min >= 0.0 && x_max > x_min)
    {
      collision_result result { glm::vec3(1.0, 0.0, 0.0), x_min };
      return result;
    }
  }

  // Vertical collision
  const auto TOLERANCE_Y = 0.1;
  if (x_max >= TOLERANCE_Y && x_min >= TOLERANCE_Y)
  {
    if (y_max >= 0.0 && y_min > y_max)
    {
      // Counteract A to move up (Y-)
      collision_result result { glm::vec3(0.0, -1.0, 0.0), y_max };
      return result;
    }
    else if (y_min >= 0.0 && y_max > y_min)
    {
      // Counteract A to move down (Y+)
      collision_result result { glm::vec3(0.0, 1.0, 0.0), y_min };
      return result;
    }
  }

  // Nothing collided
  return std::nullopt;
}

void addCollisionsCount(std::array<float, 4> &numCollisions, glm::vec3 normal)
{
  numCollisions[0] += std::max(glm::dot(glm::vec3(1.0, 0.0, 0.0), normal), 0.0f);
  numCollisions[1] += std::max(glm::dot(glm::vec3(-1.0, 0.0, 0.0), normal), 0.0f);
  numCollisions[2] += std::max(glm::dot(glm::vec3(0.0, 1.0, 0.0), normal), 0.0f);
  numCollisions[3] += std::max(glm::dot(glm::vec3(0.0, -1.0, 0.0), normal), 0.0f);
}

float getCollisionCount(std::array<float, 4> &numCollisions, glm::vec3 normal)
{
  float count = 0;
  count += std::max(glm::dot(glm::vec3(1.0, 0.0, 0.0), normal), 0.0f) * numCollisions[0];
  count += std::max(glm::dot(glm::vec3(-1.0, 0.0, 0.0), normal), 0.0f) * numCollisions[1];
  count += std::max(glm::dot(glm::vec3(0.0, 1.0, 0.0), normal), 0.0f) * numCollisions[2];
  count += std::max(glm::dot(glm::vec3(0.0, -1.0, 0.0), normal), 0.0f) * numCollisions[3];
  return count;
}

void physics::updatePhysics(game &game)
{
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
  for (auto entity : query)
  {
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

    physics_val.numCollisions = {
        0,
    };
  }

  // Collision check phase
  for (auto a_entity : query)
  {
    auto [a_physics, a_transform, a_boundary] = a_entity->get<physics, transform, boundary>();

    // Collision check
    auto a_rect = a_boundary.getWorldRect(a_transform);
    tile_index::tile min{
        static_cast<int32_t>(std::floor(a_rect.min.x)),
        static_cast<int32_t>(std::floor(a_rect.min.y)),
    };
    tile_index::tile max{
        static_cast<int32_t>(std::ceil(a_rect.max.x)),
        static_cast<int32_t>(std::ceil(a_rect.max.y)),
    };
    for (int y = min[1]; y < max[1]; y += 1)
    {
      for (int x = min[0]; x < max[0]; x += 1)
      {
        tile_index::tile current{x, y};
        auto targets = game.mWorld.getTileIndex().get(current);
        for (auto target_id : targets)
        {
          // Skip itself for obvious reasons
          if (target_id == a_entity->getId())
            continue;
          auto b_entity = game.mWorld.get(target_id);
          auto [b_transform, b_boundary] = b_entity->get<transform, boundary>();
          auto b_physics = b_entity->try_get<physics>();
          auto b_rect = b_boundary.getWorldRect(b_transform);

          // If the other entity has physics too, compare entity ID to avoid
          // running same collision twice
          if (b_physics != nullptr && target_id < a_entity->getId())
            continue;

          // Run collision test
          auto result = intersectRectRect(a_rect, b_rect);
          if (!result.has_value())
            continue;
          
          auto a_normal = result.value().normal;

          glm::vec3 ab_velocity;
          if (b_physics != nullptr)
          {
            ab_velocity = a_physics.velocity - b_physics->velocity;
          }
          else
          {
            ab_velocity = a_physics.velocity;
          }

          auto vrn = glm::dot(ab_velocity, a_normal);

          if (vrn > 0.0)
            continue;

          collisions.push_back({
            target1 : a_entity->getId(),
            target2 : target_id,
            normal : a_normal,
            length : result.value().length,
          });

          auto a_normal_dir = glm::normalize(a_normal);

          addCollisionsCount(a_physics.numCollisions, a_normal_dir);
          if (b_physics != nullptr)
          {
            addCollisionsCount(b_physics->numCollisions, -a_normal_dir);
          }
        }
      }
    }
  }
  // Impulse generation phase
  for (auto collision : collisions)
  {
    auto a_entity = game.mWorld.get(collision.target1);
    auto b_entity = game.mWorld.get(collision.target2);

    if (a_entity == nullptr || b_entity == nullptr)
    {
      // This can't happen
      continue;
    }

    auto [a_physics, a_transform, a_boundary] = a_entity->get<physics, transform, boundary>();
    auto [b_transform, b_boundary] = b_entity->get<transform, boundary>();
    auto b_physics = b_entity->try_get<physics>();

    glm::vec3 ab_velocity;
    if (b_physics != nullptr)
    {
      ab_velocity = a_physics.velocity - b_physics->velocity;
    }
    else
    {
      ab_velocity = a_physics.velocity;
    }

    auto vrn = glm::dot(ab_velocity, collision.normal);
    if (vrn > 0.0) continue;

    // Cancel movement
    if (b_physics != nullptr)
    {
      // Get ratio between A and B, and move them accordingly
      auto a_vrn = glm::abs(glm::dot(a_physics.velocity, collision.normal));
      auto b_vrn = glm::abs(glm::dot(b_physics->velocity, collision.normal));
      auto sum_vrn = a_vrn + b_vrn;
      a_transform.position += collision.normal * collision.length * (a_vrn / sum_vrn);
      b_transform.position -= collision.normal * collision.length * (b_vrn / sum_vrn);
      game.mWorld.markDirty(*b_entity);
    }
    else
    {
      a_transform.position += collision.normal * collision.length;
    }

    float collision_count = getCollisionCount(a_physics.numCollisions, collision.normal);
    // Generate impulse energy
    if (b_physics != nullptr)
    {
      const float RESTITUTION_COEFF = 0.0;
      float j = (-(1.0 + RESTITUTION_COEFF) * vrn) /
                (1.0 / a_physics.mass + 1.0 / b_physics->mass);
      a_physics.force += collision.normal * j / collision_count;

      float b_collision_count = getCollisionCount(b_physics->numCollisions, -collision.normal);
      b_physics->force -= collision.normal * j / b_collision_count;
    }
    else
    {
      float impact_energy = -vrn * (1.0) * a_physics.mass;
      auto fi = collision.normal * impact_energy;
      a_physics.force += fi / collision_count;
    }

    a_physics.onGround = -10;

    // Update collision grid right away
    game.mWorld.markDirty(*a_entity);
  }
  game.mWorld.updateIndex();
}
