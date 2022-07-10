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
    
    inline bool operator==(Entity& entity) const;
};

class EntityStore {
  private:
    EntityId idCounter = 0;
  public:
    std::list<Entity> entityList {};
    
    Entity& create();
    void remove(Entity& entity);
    std::list<Entity>::iterator begin();
    std::list<Entity>::iterator end();
};

#endif // GAME_HPP_
