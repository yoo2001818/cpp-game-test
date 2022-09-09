#include "game.hpp"
#include "entity.hpp"
#include "transform.hpp"
#include "velocity.hpp"
#include "tile.hpp"
#include "physics.hpp"
#include "boundary.hpp"

struct player {};

void game::init() {
  tile::loadTile(*this);
  {
    auto entity = mWorld.create();
    auto& transform_val = entity->set<transform>();
    transform_val.position.x = 10.5;
    transform_val.position.y = 0;
    entity->set<physics::physics>();
    entity->set<boundary>();
    entity->set<player>();
  }
}

void game::update() {
  int windowWidth, windowHeight;
  SDL_GetRendererOutputSize(mRenderer, &windowWidth, &windowHeight);

  auto query = mWorld.getQuery<player>();
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

  tile::updateTile(*this);
  physics::updatePhysics(*this);

  for (auto entity : query) {
    auto [physics_val, transform_val] = entity->get<physics::physics, transform>();
    mViewport.mTransform.position.x = transform_val.position.x - windowWidth / 2 / 36.0;
    mViewport.mTransform.position.y = transform_val.position.y - windowHeight / 2 / 36.0;
  }

  SDL_PumpEvents();
  int mouseX;
  int mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);

  // mViewport.mTransform.position.x = -(mouseX - windowWidth / 2) / 36.0;
  // mViewport.mTransform.position.y = -(mouseY - windowHeight / 2) / 36.0;
}

void game::render() {
  SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

  SDL_RenderClear(mRenderer);

  int windowWidth, windowHeight;
  SDL_GetRendererOutputSize(mRenderer, &windowWidth, &windowHeight);

  tile::renderTile(*this);

  // Render non-tile entities
  float offsetX = mViewport.mTransform.position.x;
  float offsetY = mViewport.mTransform.position.y;
  for (auto entity : mWorld) {
    auto transform_val = entity->try_get<transform>();
    if (transform_val == nullptr) {
      continue;
    }
    if (entity->has<tile::tile>()) continue;
    SDL_Rect rect;
    rect.x = (transform_val->position.x - offsetX) * 36;
    rect.y = (transform_val->position.y - offsetY) * 36;
    rect.w = 36;
    rect.h = 36;
    SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
    SDL_RenderFillRect(mRenderer, &rect);
  }

  // Render "minimap" on the rendered list
  for (auto entity : mWorld) {
    auto transform_val = entity->try_get<transform>();
    if (transform_val == nullptr) {
      continue;
    }
    SDL_Rect rect;
    rect.x = transform_val->position.x * 3;
    rect.y = transform_val->position.y * 3;
    rect.w = 3;
    rect.h = 3;
    SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
    SDL_RenderFillRect(mRenderer, &rect);
  }

  SDL_RenderPresent(mRenderer);
}

void game::handleEvent(const SDL_Event& event) {
}
