#include <map>
#include <typeindex>
#include <any>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct transform {
  alignas(16) glm::vec3 position {0., 0., 0.};
  alignas(16) glm::vec3 scale {1., 1., 1.};
  alignas(16) glm::quat rotation {0., 0., 0., 1.};
};

class entity {
  public:
  template<class T> T* get() {
    std::type_index type_index(typeid(T));
    auto it = this->mComponentMap.find(type_index);
    if (it == this->mComponentMap.end()) {
      return nullptr;
    }
    return &std::any_cast<T&>(it->second);
  };
  template<class T> T& set(const T& value) {
    std::type_index type_index(typeid(T));
    auto [it, is_created] = this->mComponentMap.emplace(std::pair(
      type_index,
      std::make_any<T>(value)));
    if (!is_created) {
      std::any& handle = it->second;
      handle.emplace<T>(value);
    }
    return std::any_cast<T&>(it->second);
  };
  template<class T> void remove() {
    std::type_index type_index(typeid(T));
    this->mComponentMap.erase(type_index);
  };
  template<class T> bool has() {
    std::type_index type_index(typeid(T));
    return this->mComponentMap.find(type_index) != this->mComponentMap.end();
  };

  private:
  std::map<std::type_index, std::any> mComponentMap;
};
