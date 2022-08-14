#include <catch2/catch_all.hpp>
#include <iostream>

#include "entity.hpp"
#include "transform.hpp"
#include "world.hpp"

TEST_CASE("World stores entiites", "[world]") {
  world world;
  {
    entity entity;
    world.add(std::move(entity));
  }
}

