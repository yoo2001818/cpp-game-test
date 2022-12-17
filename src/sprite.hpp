#ifndef SPRITE_HPP_
#define SPRITE_HPP_

#include <cstdint>
#include <string>

#include "game.hpp"

namespace sprite {
  class sprite {
    public:
    std::string tileset;
    int32_t id;
  };

  void renderSprite(game& game);
}

#endif // SPRITE_HPP_

