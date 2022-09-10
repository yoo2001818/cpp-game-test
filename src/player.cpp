#include "player.hpp"
#include "physics.hpp"

void player::updatePlayer(game& game) {
  auto query = game.mWorld.getQuery<player>();
  const uint8_t* keyState = SDL_GetKeyboardState(nullptr);
  for (auto entity : query) {
    auto [physics_val, transform_val] = entity->get<physics::physics, transform>();
    if (keyState[SDL_SCANCODE_LEFT]) {
      physics_val.force.x -= 0.01;
    } else if (keyState[SDL_SCANCODE_RIGHT]) {
      physics_val.force.x += 0.01;
    } else {
      physics_val.force.x -= physics_val.velocity.x * 0.05 / physics_val.mass;
    }
    if (keyState[SDL_SCANCODE_SPACE]) {
      if (std::abs(physics_val.velocity.y) < 0.001) {
        physics_val.velocity.y = -0.3;
      }
    }
  }

  int windowWidth, windowHeight;
  SDL_GetRendererOutputSize(game.mRenderer, &windowWidth, &windowHeight);

  for (auto entity : query) {
    auto [physics_val, transform_val] = entity->get<physics::physics, transform>();
    game.mViewport.mTransform.position.x = transform_val.position.x - windowWidth / 2 / 36.0;
    game.mViewport.mTransform.position.y = transform_val.position.y - windowHeight / 2 / 36.0;
  }
}
