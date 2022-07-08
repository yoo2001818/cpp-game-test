#include <list>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform {
  alignas(16) glm::vec3 position;
  alignas(16) glm::vec3 scale;
  alignas(16) glm::quat rotation;
};

class Entity {
  public:
    Transform transform;
    std::string name;
};

class EntityStore {
  public:
    std::list<Entity> entityList;
};
