#include "world.hpp"

world::entity &world::world::create() {
  entity_id id = this->mIdCounter;
  this->mIdCounter++;
  this->mEntities.push_back({});
  entity &mEntity = this->mEntities.back();
  mEntity.mId = id;
  return mEntity;
}
