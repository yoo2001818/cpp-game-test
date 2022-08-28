#ifndef TILE_INDEX_HPP_
#define TILE_INDEX_HPP_

#include <map>
#include <cstdint>
#include <set>
#include <array>

#include "entity.hpp"

class world;

class tile_index {
  public:
  using tile = std::array<int32_t, 2>;

  tile_index(world& pWorld):
    mWorld(pWorld),
    mTileMap(),
    mOccupiedTiles() {};
  void remove(const entity_id& id);
  void insert(entity& entity);
  void update(const entity_id& id);
  std::set<entity_id>& get(const tile& tile);

  private:
  std::map<tile, std::set<entity_id>> mTileMap;
  std::map<entity_id, std::set<tile>> mOccupiedTiles;
  world& mWorld;
};

#endif // TILE_INDEX_HPP_
