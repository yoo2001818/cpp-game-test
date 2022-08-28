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
    entity_val->mIsDirty = false;
    this->mEntityCount += 1;

    return entity_val;
  } else {
    std::shared_ptr<entity> entity_val = mDeadEntityList.top();
    mDeadEntityList.pop();
    *entity_val = std::move(pEntity);

    // Increment version
    entity_val->mIsAlive = true;
    entity_val->mIsDirty = false;
    entity_val->mEntityId.version += 1;

    return entity_val;
  }
}

void world::remove(const entity& target) {
  this->remove(target.getId());
}

void world::remove(const entity_id& id) {
  std::shared_ptr<entity> target = this->get(id);
  if (target == nullptr) return;
  this->mDeadEntityList.push(target);
  target->mIsAlive = false;
  // Dead entity is marked dirty also
  this->mDirtyEntityList.push(id);
  target->mIsDirty = true;
}

std::shared_ptr<entity> world::get(const entity_id& id) const {
  std::shared_ptr<entity> target = mEntityList.at(id.id);
  if (!target->isAlive()) return nullptr;
  if (target->getId().version != id.version) return nullptr;
  return std::move(target);
}

world::entity_iterator world::begin() {
  return world::entity_iterator(this->mEntityList.begin(), this->mEntityList.end(), {});
}

world::entity_iterator world::end() {
  return world::entity_iterator(this->mEntityList.end(), this->mEntityList.end(), {});
}

void world::markDirty(const entity& entity) {
  this->markDirty(entity.getId());
}

void world::markDirty(const entity_id& id) {
  std::shared_ptr<entity> target = this->get(id);
  if (target == nullptr) return;
  if (target->isDirty()) return;
  this->mDirtyEntityList.push(id);
  target->mIsDirty = true;
}

void world::updateIndex() {
  while (!this->mDirtyEntityList.empty()) {
    entity_id& id = this->mDirtyEntityList.top();
    mTileIndex.update(id);
    this->mDirtyEntityList.pop();
  }
}

tile_index& world::getTileIndex() {
  return mTileIndex;
}
