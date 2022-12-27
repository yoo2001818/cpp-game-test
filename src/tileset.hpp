#ifndef TILESET_HPP_
#define TILESET_HPP_

#include <cstdint>
#include <string>
#include <SDL2/SDL_image.h>

namespace tileset {
  class tileset_metadata {
    public:
    int tileWidth;
    int tileHeight;
    int width;
    int height;
  };

  class tileset {
    public:
    std::string name;
    SDL_Texture* texture;
    tileset_metadata metadata;

    tileset(SDL_Renderer* renderer, std::string path, tileset_metadata metadata);
    ~tileset();
  };

}

#endif // TILESET_HPP_


