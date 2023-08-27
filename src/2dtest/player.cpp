#include <iostream>

#include "player.hpp"
#include "physics.hpp"
#include "sprite.hpp"

void player::updatePlayer(game& game) {
  auto query = game.mWorld.getQuery<player>();
  const uint8_t* keyState = SDL_GetKeyboardState(nullptr);
  for (auto entity : query) {
    auto [physics_val, transform_val] = entity->get<physics::physics, transform>();
    if (keyState[SDL_SCANCODE_LEFT]) {
      physics_val.force.x -= 0.6;
    } else if (keyState[SDL_SCANCODE_RIGHT]) {
      physics_val.force.x += 0.6;
    } else {
      physics_val.force.x -= physics_val.velocity.x * 0.06 / physics_val.mass;
    }
    if (keyState[SDL_SCANCODE_SPACE]) {
      if (physics_val.onGround <= 0) {
        physics_val.velocity.y = -18.0;
        physics_val.onGround = 1;
      }
    }
    if (game.mTick % 10 == 0) {
      auto& sprite_val = entity->get<sprite::sprite>();
      sprite_val.id = (sprite_val.id + 1) % 4;
    }
  }

  int windowWidth, windowHeight;
  SDL_GetRendererOutputSize(game.mRenderer, &windowWidth, &windowHeight);

  for (auto entity : query) {
    auto [physics_val, transform_val] = entity->get<physics::physics, transform>();
    game.mViewport.mTransform.position.x = transform_val.position.x - windowWidth / 2 / 36.0;
    game.mViewport.mTransform.position.y = transform_val.position.y - windowHeight / 2 / 36.0;
    // std::cout << transform_val.position.y << ", " << physics_val.velocity.y << ", " << physics_val.force.y << std::endl;
  }
}
