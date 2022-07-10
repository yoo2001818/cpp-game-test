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

std::list<Entity>::iterator EntityStore::begin() {
  return this->entityList.begin();
}

std::list<Entity>::iterator EntityStore::end() {
  return this->entityList.end();
}

inline bool Entity::operator==(Entity& entity) const {
  return this->id == entity.id;
}
