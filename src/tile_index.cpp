#include <cmath>
#include "tile_index.hpp"
#include "world.hpp"
#include "boundary.hpp"
#include "transform.hpp"

void tile_index::remove(const entity_id& id) {
  // Loop for each occupied tile, then remove the entity from tilemap
  auto it = mOccupiedTiles.find(id);
  if (it == mOccupiedTiles.end()) return;
  for (auto tile : it->second) {
    auto it = mTileMap.find(tile);
    if (it == mTileMap.end()) continue;

    auto tile_entities = it->second;
    tile_entities.erase(id);

    if (tile_entities.empty()) {
      mTileMap.erase(it);
    }
  }
  mOccupiedTiles.erase(it);
}

void tile_index::insert(entity& entity) {
  // NOTE: This does not support any matrix operations
  const entity_id& id = entity.getId();
  auto transform_val = entity.get<transform>();
  if (transform_val == nullptr) return;
  auto boundary_val = entity.get<boundary>();
  if (boundary_val == nullptr) return;
  
  tile_index::tile min {
    static_cast<int32_t>(std::floor(transform_val->position.x + boundary_val->min.x)),
    static_cast<int32_t>(std::floor(transform_val->position.y + boundary_val->min.y)),
  };
  tile_index::tile max {
    static_cast<int32_t>(std::ceil(transform_val->position.x + boundary_val->max.x)),
    static_cast<int32_t>(std::ceil(transform_val->position.y + boundary_val->max.y)),
  };
  std::set<tile_index::tile> occupiedTiles;
  for (int y = min[1]; y < max[1]; y += 1) {
    for (int x = min[0]; x < max[0]; x += 1) {
      tile_index::tile current {x, y};
      auto [it, is_created] = mTileMap.emplace(std::pair(current, std::set<entity_id>()));
      it->second.insert(id);
      occupiedTiles.insert(current);
    }
  }
  mOccupiedTiles.emplace(std::pair(id, occupiedTiles));
}

void tile_index::update(const entity_id& id) {
  std::shared_ptr<entity> target = mWorld.get(id);
  this->remove(id);
  if (target != nullptr) this->insert(*target);
}

std::set<entity_id> empty_entity_set;

std::set<entity_id>& tile_index::get(const tile_index::tile& tile) {
  auto it = mTileMap.find(tile);
  if (it == mTileMap.end()) return empty_entity_set;
  return it->second;
}
