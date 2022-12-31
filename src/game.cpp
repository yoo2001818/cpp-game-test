#include <SDL2/SDL_image.h>
#include "game.hpp"
#include "entity.hpp"
#include "transform.hpp"
#include "velocity.hpp"
#include "tile.hpp"
#include "sprite.hpp"
#include "physics.hpp"
#include "boundary.hpp"
#include "player.hpp"

void game::init() {
  mTileResourceManager.insert("tile2",
    std::make_shared<tileset::tileset>(
      mRenderer,
      "res/tile2.png",
      tileset::tileset_metadata({ 18, 18, 20, 9 })));
  mTileResourceManager.insert("player-idle",
    std::make_shared<tileset::tileset>(
      mRenderer,
      "res/player-idle.png",
      tileset::tileset_metadata({ 33, 32, 4, 1 })));
  tile::loadTile(*this);
  {
    auto entity = mWorld.create();
    auto& transform_val = entity->set<transform>();
    transform_val.position.x = 10.5;
    transform_val.position.y = 0;
    auto& physics_val = entity->set<physics::physics>();
    physics_val.hasCollisionHandler = true;
    auto& boundary_val = entity->set<boundary>();
    boundary_val.rect.max = glm::vec3(1.0, 1.0, 1.0);
    entity->set<player::player>();
    entity->set<sprite::sprite>({ "player-idle", 0, { glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0) } });
  }
  {
    // box
    auto entity = mWorld.create();
    auto& transform_val = entity->set<transform>();
    transform_val.position.x = 13.5;
    transform_val.position.y = 0;
    auto& physics_val = entity->set<physics::physics>();
    physics_val.hasCollisionHandler = true;
    entity->set<boundary>();
    entity->set<sprite::sprite>({ "tile2", 26 });
  }
  {
    // big box
    auto entity = mWorld.create();
    auto& transform_val = entity->set<transform>();
    transform_val.position.x = 16.5;
    transform_val.position.y = 0;
    auto& physics_val = entity->set<physics::physics>();
    physics_val.hasCollisionHandler = true;
    auto& boundary_val = entity->set<boundary>();
    boundary_val.rect.max = glm::vec3(2.0, 2.0, 2.0);
    entity->set<sprite::sprite>({ "tile2", 26 });
  }
}

void game::update() {
  if (!this->mPaused || this->mShouldStep) {
    tile::updateTile(*this);
    player::updatePlayer(*this);
    physics::updatePhysics(*this);
    this->mTick += 1;
    this->mShouldStep = false;
  }

  SDL_PumpEvents();
}

void game::render() {
  SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

  SDL_RenderClear(mRenderer);

  int windowWidth, windowHeight;
  SDL_GetRendererOutputSize(mRenderer, &windowWidth, &windowHeight);

  tile::renderTile(*this);
  sprite::renderSprite(*this);

  // Render non-tile entities
  float offsetX = mViewport.mTransform.position.x;
  float offsetY = mViewport.mTransform.position.y;
  for (auto entity : mWorld) {
    auto transform_val = entity->try_get<transform>();
    if (transform_val == nullptr) {
      continue;
    }
    // if (entity->has<tile::tile>()) continue;
    // if (entity->has<sprite::sprite>()) continue;
    auto boundary_val = entity->try_get<boundary>();
    SDL_Rect rect;
    rect.x = static_cast<int>(std::roundf((transform_val->position.x - offsetX) * 36.0));
    rect.y = static_cast<int>(std::roundf((transform_val->position.y - offsetY) * 36.0));
    rect.w = 36;
    rect.h = 36;
    if (boundary_val != nullptr) {
      rect.x -= boundary_val->rect.min.x * 36;
      rect.y -= boundary_val->rect.min.y * 36;
      rect.w = boundary_val->rect.max.x * 36;
      rect.h = boundary_val->rect.max.y * 36;
    }
    SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(mRenderer, &rect);
  }

  // Render collisions
  for (auto entity : mWorld) {
    auto transform_val = entity->try_get<transform>();
    auto physics_val = entity->try_get<physics::physics>();
    auto boundary_val = entity->try_get<boundary>();
    if (transform_val == nullptr || physics_val == nullptr) {
      continue;
    }
    SDL_SetRenderDrawColor(mRenderer, 0, 128, 128, 255);
    float x = transform_val->position.x - offsetX;
    float y = transform_val->position.y - offsetY;
    if (boundary_val != nullptr) {
      x += (boundary_val->rect.min.x + boundary_val->rect.max.x) / 2.0;
      y += (boundary_val->rect.min.y + boundary_val->rect.max.y) / 2.0;
    }
    SDL_RenderDrawLineF(
      mRenderer,
      x * 36.0,
      y * 36.0,
      (x + physics_val->velocity.x / 6.0) * 36.0,
      (y + physics_val->velocity.y / 6.0) * 36.0
    );
    for (auto collision : physics_val->collisions) {
      SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
      SDL_Rect rect {
        (collision.position.x - offsetX) * 36.0 - 8,
        (collision.position.y - offsetY) * 36.0 - 8,
        16,
        16
      };
      SDL_RenderFillRect(mRenderer, &rect);
      SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
      SDL_RenderDrawLineF(
        mRenderer,
        rect.x,
        rect.y,
        rect.x + collision.direction.x * 36.0,
        rect.y + collision.direction.y * 36.0
      );
    }
  }

  SDL_RenderPresent(mRenderer);
}

void game::handleEvent(const SDL_Event& event) {
  switch (event.type) {
    case SDL_EventType::SDL_KEYDOWN: {
      switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          this->mPaused = !this->mPaused;
          break;
        case SDLK_e:
          this->mShouldStep = true;
          break;
      }
      break;
    }
  }
}
