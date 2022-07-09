#ifndef GAME_HPP_
#define GAME_HPP_

#include <list>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform {
  alignas(16) glm::vec3 position {0., 0., 0.};
  alignas(16) glm::vec3 scale {1., 1., 1.};
  alignas(16) glm::quat rotation {0., 0., 0., 1.};
};

typedef int EntityId;

class Entity {
  public:
    EntityId id;
    Transform transform;
    std::string name;
};

class EntityStore {
  private:
    EntityId idCounter = 0;
  public:
    std::list<Entity> entityList {};
    
    Entity& create();
    void remove(Entity& entity);
};

#endif // GAME_HPP_
