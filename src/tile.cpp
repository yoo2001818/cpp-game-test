#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "tile.hpp"
#include "transform.hpp"

SDL_Texture *TILE_TEXTURE;

void tile::loadTile(game& game) {
  // TODO: Move this to asset manager
  TILE_TEXTURE = IMG_LoadTexture(game.mRenderer, "res/tile2.png");
  if (TILE_TEXTURE == nullptr) {
    throw std::runtime_error("Failed to load tile image");
  }

  // Read map file
  std::ifstream mapFile;
  mapFile.open("res/map2.csv");
  
  int y = 0;
  std::string line;
  while (std::getline(mapFile, line)) {
    int x = 0;
    std::stringstream lineStream(line);
    std::string cell;
    
    while (std::getline(lineStream, cell, ',')) {
      int id = std::stoi(cell);
      if (id > -1) {
        auto entity = game.mWorld.create();
        auto& transform_val = entity->set<transform>();
        transform_val.position.x = static_cast<float>(x);
        transform_val.position.y = static_cast<float>(y);

        auto& tile_val = entity->set<tile>({ id });
        game.mWorld.markDirty(*entity);
      }
      x += 1;
    }
    y += 1;
  }
  game.mWorld.updateIndex();
}

void tile::updateTile(game& game) {

}

const int TILE_SIZE = 36;

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
    srcRect.x = (tile_val->id % 20) * 18;
    srcRect.y = (tile_val->id / 20) * 18;
    srcRect.w = 18;
    srcRect.h = 18;
    SDL_RenderCopy(game.mRenderer, TILE_TEXTURE, &srcRect, &rect);
  }
}
