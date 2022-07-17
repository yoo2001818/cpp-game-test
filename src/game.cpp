#include <algorithm>

#include "game.hpp"

std::shared_ptr<Entity>& EntityStore::create() {
  std::shared_ptr<Entity> entity = std::make_shared<Entity>();
  entity->id = idCounter;
  entity->name = "Entity";
  idCounter += 1;

  entityList.emplace_back(std::move(entity));
  return entityList.back();
}

void EntityStore::remove(std::shared_ptr<Entity>& entity) {
  entityList.remove(entity);
}

std::list<std::shared_ptr<Entity>>::iterator EntityStore::begin() {
  return this->entityList.begin();
}

std::list<std::shared_ptr<Entity>>::iterator EntityStore::end() {
  return this->entityList.end();
}

inline bool Entity::operator==(Entity& entity) const {
  return this->id == entity.id;
}
