#include <algorithm>

#include "game.hpp"

std::unique_ptr<Entity>& EntityStore::create() {
  std::unique_ptr<Entity> entity = std::make_unique<Entity>();
  entity->id = idCounter;
  entity->name = "Entity";
  idCounter += 1;

  entityList.emplace_back(std::move(entity));
  return entityList.back();
}

void EntityStore::remove(std::unique_ptr<Entity>& entity) {
  entityList.remove(entity);
}

std::list<std::unique_ptr<Entity>>::iterator EntityStore::begin() {
  return this->entityList.begin();
}

std::list<std::unique_ptr<Entity>>::iterator EntityStore::end() {
  return this->entityList.end();
}

inline bool Entity::operator==(Entity& entity) const {
  return this->id == entity.id;
}
