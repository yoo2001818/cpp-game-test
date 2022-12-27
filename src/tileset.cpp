#include "tileset.hpp"

tileset::tileset::tileset(SDL_Renderer* renderer, std::string path, tileset_metadata metadata) {
  this->name = name;
  this->metadata = metadata;

  this->texture = IMG_LoadTexture(renderer, path.c_str());
}

tileset::tileset::~tileset() {
  SDL_DestroyTexture(this->texture);
}
