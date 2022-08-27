#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <vector>
#include <memory>
#include <stack>
#include <cstdint>
#include "entity.hpp"

class world {
  public:
  std::vector<std::shared_ptr<entity>> mEntityList;

  std::shared_ptr<entity> create();
  std::shared_ptr<entity> add(entity&& entity);
  void remove(const entity& entity);
  void remove(const entity_id& entity);
  std::vector<std::shared_ptr<entity>>::iterator begin();
  std::vector<std::shared_ptr<entity>>::iterator end();
  entity* get(entity_id id);

  private:
  std::stack<std::shared_ptr<entity>> mDeadEntityList;
  uint32_t mEntityCount;
};

#endif // WORLD_HPP_
