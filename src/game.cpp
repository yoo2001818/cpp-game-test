#include <algorithm>

#include "game.hpp"

Entity& EntityStore::create() {
  Entity entity;
  entity.id = idCounter;
  entity.name = "Entity";
  idCounter += 1;

  entityList.emplace_back(std::move(entity));
  return entityList.back();
}

void EntityStore::remove(Entity& entity) {
  entityList.remove(entity);
}
