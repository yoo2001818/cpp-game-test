#include <algorithm>
#include "world.hpp"

std::shared_ptr<entity> world::create() {
  entity entity;
  return world::add(std::move(entity));
}

std::shared_ptr<entity> world::add(entity&& pEntity) {
  std::shared_ptr<entity> entity_val = std::make_shared<entity>(pEntity);
  
  this->mEntityList.push_back(entity_val);
  return entity_val;
}

void world::remove(const std::shared_ptr<entity>& entity) {
  for (auto it = this->begin(); it != this->end(); it++) {
    if (*it == entity) {
      it++;
      this->mEntityList.erase(it);
    }
  }
}

std::vector<std::shared_ptr<entity>>::iterator world::begin() {
  return this->mEntityList.begin();
}

std::vector<std::shared_ptr<entity>>::iterator world::end() {
  return this->mEntityList.end();
}

