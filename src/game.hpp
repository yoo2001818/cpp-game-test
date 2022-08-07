#ifndef GAME_HPP_
#define GAME_HPP_

#include <list>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform {
  alignas(16) glm::vec3 position {0., 0., 0.};
  alignas(16) glm::vec3 scale {1., 1., 1.};
  alignas(16) glm::quat rotation {0., 0., 0., 1.};
};

typedef int EntityId;
typedef int TileId;

class Entity {
  public:
    EntityId id;
    Transform transform;
    glm::vec3 velocity {0., 0., 0.};
    std::string name;
    
    inline bool operator==(Entity& entity) const;
};

class Tile {
  public:
    TileId tileId;
};

class TileMap {
  private:
    std::vector<std::vector<Tile>> tiles;
};

class World {
  private:
    EntityId idCounter = 0;
  public:
    std::list<std::shared_ptr<Entity>> entityList {};
    
    std::shared_ptr<Entity>& create();
    void remove(std::shared_ptr<Entity>& entity);
    std::list<std::shared_ptr<Entity>>::iterator begin();
    std::list<std::shared_ptr<Entity>>::iterator end();
};

#endif // GAME_HPP_
