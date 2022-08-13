#include <catch2/catch_all.hpp>

#include "entity.hpp"

TEST_CASE("Entity stores an object", "[entity]") {
  entity entity_item;
  REQUIRE(entity_item.has<transform>() == false);
  entity_item.set<transform>({});
  REQUIRE(entity_item.has<transform>() == true);
}
