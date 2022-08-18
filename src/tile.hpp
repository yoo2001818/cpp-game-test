#ifndef TILE_HPP_
#define TILE_HPP_

#include <cstdint>

#include "game.hpp"

namespace tile {
  class tile {
    public:
    int32_t id;
  };

  void loadTile(game& game);
  void updateTile(game& game);
  void renderTile(game& game);
}

#endif // TILE_HPP_
