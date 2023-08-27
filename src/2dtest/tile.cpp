#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "tile.hpp"
#include "transform.hpp"
#include "boundary.hpp"

void tile::loadTile(game& game) {
  // Read map file
  std::ifstream mapFile;
  mapFile.open("res/map3.csv");
  
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

        auto& tile_val = entity->set<tile>({ "tile2", id });
        entity->set<boundary>();

        game.mWorld.markDirty(*entity);
      }
      x += 1;
    }
    y += 1;
  }
  game.mWorld.updateIndex();
}

void tile::updateTile(game& game) {
  // std::cout << game.mWorld.getTileIndex().mTileMap.size() << std::endl;
}

const int TILE_SIZE = 36;

void tile::renderTile(game& game) {
  int windowWidth, windowHeight;
  SDL_GetRendererOutputSize(game.mRenderer, &windowWidth, &windowHeight);

  float offsetX = game.mViewport.mTransform.position.x;
  float offsetY = game.mViewport.mTransform.position.y;
  int tileOffsetX = static_cast<int32_t>(std::floor(offsetX));
  int tileOffsetY = static_cast<int32_t>(std::floor(offsetY));
  float deltaX = std::fmod(offsetX, 1.0);
  float deltaY = std::fmod(offsetY, 1.0);
  if (deltaX < 0.0) deltaX += 1.0;
  if (deltaY < 0.0) deltaY += 1.0;
  int tileDeltaX = static_cast<int32_t>(std::floor(deltaX * TILE_SIZE));
  int tileDeltaY = static_cast<int32_t>(std::floor(deltaY * TILE_SIZE));
  int tileWidth = (windowWidth + TILE_SIZE - 1) / TILE_SIZE + 1;
  int tileHeight = (windowHeight + TILE_SIZE - 1) / TILE_SIZE + 1;

  for (int y = 0; y < tileHeight; y += 1) {
    for (int x = 0; x < tileWidth; x += 1) {
      auto entity_set = game.mWorld.getTileIndex().get({x + tileOffsetX, y + tileOffsetY});
      for (auto entity_id : entity_set) {
        auto entity = game.mWorld.get(entity_id);
        if (entity == nullptr) continue;
        auto [transform_val, tile_val] = entity->try_get<transform, tile>();
        if (transform_val == nullptr) continue;
        if (tile_val == nullptr) continue;

        auto& tile_tex_opt = game.mTileResourceManager.get(tile_val->tileset).value();
        if (!tile_tex_opt.has_value()) continue;

        auto& tile_tex = tile_tex_opt.value();

        SDL_Rect rect;
        rect.x = static_cast<int>(std::roundf((transform_val->position.x - tileOffsetX) * TILE_SIZE - tileDeltaX));
        rect.y = static_cast<int>(std::roundf((transform_val->position.y - tileOffsetY) * TILE_SIZE - tileDeltaY));
        rect.w = TILE_SIZE;
        rect.h = TILE_SIZE;
        SDL_Rect srcRect;
        srcRect.x = (tile_val->id % tile_tex->metadata.width) * tile_tex->metadata.tileWidth;
        srcRect.y = (tile_val->id / tile_tex->metadata.width) * tile_tex->metadata.tileHeight;
        srcRect.w = tile_tex->metadata.tileWidth;
        srcRect.h = tile_tex->metadata.tileHeight;
        SDL_RenderCopy(game.mRenderer, tile_tex->texture, &srcRect, &rect);
      }
    }
  }
}
