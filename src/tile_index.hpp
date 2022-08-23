#ifndef TILE_INDEX_HPP_
#define TILE_INDEX_HPP_

#include <list>
#include <map>
#include <cstdint>

#include "entity.hpp"

class tile_index {
  public:
    std::map<int32_t[2], std::list<entity&>> mTileMap;
};

#endif // TILE_INDEX_HPP_
