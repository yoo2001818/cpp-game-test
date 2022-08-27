#include <algorithm>
#include "world.hpp"

std::shared_ptr<entity> world::create() {
  entity entity;
  return world::add(std::move(entity));
}

std::shared_ptr<entity> world::add(entity&& pEntity) {
  if (mDeadEntityList.empty()) {
    std::shared_ptr<entity> entity_val = std::make_shared<entity>(std::move(pEntity));
    this->mEntityList.push_back(entity_val);

    // Assign IDs
    entity_val->mEntityId = { id: this->mEntityCount, version: 0 };
    entity_val->mIsAlive = true;
    this->mEntityCount += 1;

    return entity_val;
  } else {
    std::shared_ptr<entity> entity_val = mDeadEntityList.top();
    mDeadEntityList.pop();
    *entity_val = std::move(pEntity);

    // Increment version
    entity_val->mIsAlive = true;
    entity_val->mEntityId.version += 1;

    return entity_val;
  }
}

void world::remove(const entity& target) {
  this->remove(target.mEntityId);
}

void world::remove(const entity_id& target) {
  std::shared_ptr<entity> target_in_world = mEntityList.at(target.id);
  if (!target_in_world->mIsAlive) return;
  if (target_in_world->mEntityId.version != target.version) return;
  mDeadEntityList.push(target_in_world);
  target_in_world->mIsAlive = false;
}

std::shared_ptr<entity> world::get(const entity_id& id) {
  std::shared_ptr<entity> target_in_world = mEntityList.at(id.id);
  if (!target_in_world->mIsAlive) return nullptr;
  if (target_in_world->mEntityId.version != id.version) return nullptr;
  return std::move(target_in_world);
}

world::iterator world::begin() {
  return world::iterator(this->mEntityList.begin(), this->mEntityList.end());
}

world::iterator world::end() {
  return world::iterator(this->mEntityList.end(), this->mEntityList.end());
}

