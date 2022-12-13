#ifndef TILE_HPP_
#define TILE_HPP_

#include <cstdint>
#include <string>

#include "game.hpp"

namespace tile {
  class tile {
    public:
    std::string tileset;
    int32_t id;
  };

  void loadTile(game& game);
  void updateTile(game& game);
  void renderTile(game& game);
}

#endif // TILE_HPP_
