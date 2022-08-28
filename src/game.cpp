#include "game.hpp"
#include "entity.hpp"
#include "transform.hpp"
#include "velocity.hpp"
#include "tile.hpp"

void game::init() {
  tile::loadTile(*this);
}

void game::update() {
  int windowWidth, windowHeight;
  SDL_GetRendererOutputSize(mRenderer, &windowWidth, &windowHeight);

  tile::updateTile(*this);

  for (int i = 0; i < 10; i += 1) {
    auto entity = mWorld.create();
    auto& transform_val = entity->set<transform>();
    transform_val.position.x = windowWidth / 2;
    transform_val.position.y = windowHeight / 2;
    auto& velocity_val = entity->set<velocity>();
    velocity_val.value.x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0 - 1.0;
    velocity_val.value.y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0 - 1.0;
    velocity_val.value = glm::normalize(velocity_val.value);
  }

  auto query = mWorld.getQuery<transform, velocity>();
  for (auto i = query.begin(); i != query.end(); i++) {
    auto entity = *i;
    auto [transform_val, velocity_val] = entity->get<transform, velocity>();
    transform_val.position.x += velocity_val.value.x * 2.0;
    transform_val.position.y += velocity_val.value.y * 2.0;

    if (
      transform_val.position.x < 0.0 ||
      transform_val.position.x > windowWidth ||
      transform_val.position.y < 0.0 ||
      transform_val.position.y > windowHeight
    ) {
      mWorld.remove(*entity);
    }
  }
}

void game::render() {
  SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

  SDL_RenderClear(mRenderer);

  int windowWidth, windowHeight;
  SDL_GetRendererOutputSize(mRenderer, &windowWidth, &windowHeight);

  tile::renderTile(*this);

  for (auto entity : mWorld) {
    auto transform_val = entity->try_get<transform>();
    if (transform_val == nullptr) {
      continue;
    }
    SDL_Rect rect;
    rect.x = transform_val->position.x;
    rect.y = transform_val->position.y;
    rect.w = 3;
    rect.h = 3;
    SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
    SDL_RenderFillRect(mRenderer, &rect);
  }

  SDL_RenderPresent(mRenderer);
}

void game::handleEvent(const SDL_Event& event) {

}
