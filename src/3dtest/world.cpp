#include "world.hpp"

world::entity &world::world::create() {
  this->mEntities.push_back({});
  entity &mEntity = this->mEntities.back();
  return mEntity;
}
