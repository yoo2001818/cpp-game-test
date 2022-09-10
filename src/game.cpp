#include "game.hpp"
#include "entity.hpp"
#include "transform.hpp"
#include "velocity.hpp"
#include "tile.hpp"
#include "physics.hpp"
#include "boundary.hpp"
#include "player.hpp"

void game::init() {
  tile::loadTile(*this);
  {
    auto entity = mWorld.create();
    auto& transform_val = entity->set<transform>();
    transform_val.position.x = 10.5;
    transform_val.position.y = 0;
    entity->set<physics::physics>();
    entity->set<boundary>();
    entity->set<player::player>();
  }
}

void game::update() {
  tile::updateTile(*this);
  physics::updatePhysics(*this);
  player::updatePlayer(*this);

  SDL_PumpEvents();
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
