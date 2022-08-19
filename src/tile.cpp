#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "tile.hpp"
#include "transform.hpp"

SDL_Texture *TILE_TEXTURE;

void tile::loadTile(game& game) {
  // TODO: Move this to asset manager
  TILE_TEXTURE = IMG_LoadTexture(game.mRenderer, "res/tile.png");
  if (TILE_TEXTURE == nullptr) {
    throw std::runtime_error("Failed to load tile image");
  }

  for (int y = 0; y < 30; y += 1) {
    for (int x = 0; x < 30; x += 1) {
      auto entity = game.mWorld.create();
      auto& transform_val = entity->set<transform>();
      transform_val.position.x = static_cast<float>(x);
      transform_val.position.y = static_cast<float>(y);
      auto& tile_val = entity->set<tile>({ id: (x + y) % 3 });
    }
  }
}

void tile::updateTile(game& game) {

}

const int TILE_SIZE = 48;

void tile::renderTile(game& game) {
  for (auto entity : game.mWorld) {
    auto transform_val = entity->get<transform>();
    if (transform_val == nullptr) {
      continue;
    }
    auto tile_val = entity->get<tile>();
    if (tile_val == nullptr) {
      continue;
    }
    SDL_Rect rect;
    rect.x = transform_val->position.x * TILE_SIZE;
    rect.y = transform_val->position.y * TILE_SIZE;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;
    SDL_Rect srcRect;
    switch(tile_val->id) {
      case 0:
        srcRect = { 0, 0, 48, 48 };
        break;
      case 1:
        srcRect = { 48, 0, 48, 48 };
        break;
      case 2:
        srcRect = { 0, 48, 48, 48 };
        break;
    }
    SDL_RenderCopy(game.mRenderer, TILE_TEXTURE, &srcRect, &rect);
  }
}
