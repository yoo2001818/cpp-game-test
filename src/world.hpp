#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <vector>
#include <memory>
#include "entity.hpp"

class world {
  public:
  std::vector<std::shared_ptr<entity>> mEntityList;

  std::shared_ptr<entity> create();
  std::shared_ptr<entity> add(entity&& entity);
  void remove(const std::shared_ptr<entity>& entity);
  std::vector<std::shared_ptr<entity>>::iterator begin();
  std::vector<std::shared_ptr<entity>>::iterator end();
};

#endif // WORLD_HPP_
