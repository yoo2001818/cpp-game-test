#ifndef SPRITE_HPP_
#define SPRITE_HPP_

#include <cstdint>
#include <string>
#include <glm/glm.hpp>

#include "game.hpp"
#include "rect.hpp"

namespace sprite {
  class sprite {
    public:
    std::string tileset;
    int32_t id;
    ::rect rect;
  };

  void renderSprite(game& game);
}

#endif // SPRITE_HPP_

