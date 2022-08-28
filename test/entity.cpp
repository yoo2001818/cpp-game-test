#include <catch2/catch_all.hpp>
#include <iostream>

#include "entity.hpp"
#include "transform.hpp"

TEST_CASE("Entity stores an object", "[entity]") {
  entity entity;
  REQUIRE(entity.has<transform>() == false);
  transform& aa = entity.set<transform>({});
  transform& ab = entity.get<transform>();
  REQUIRE(&aa == &ab);
  REQUIRE(entity.has<transform>() == true);
  entity.remove<transform>();
  REQUIRE(entity.has<transform>() == false);
}
