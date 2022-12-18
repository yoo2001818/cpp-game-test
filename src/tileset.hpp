#ifndef TILESET_HPP_
#define TILESET_HPP_

#include <cstdint>
#include <string>
#include <SDL2/SDL_image.h>

#include "game.hpp"

namespace tileset {
  class tileset {
    public:
    std::string name;
    SDL_Texture* texture;
  };

  tileset loadTileset(std::string path);
}

#endif // TILESET_HPP_


