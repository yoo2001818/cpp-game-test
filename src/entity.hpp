#include <optional>
#include <map>
#include <typeindex>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct transform {
  alignas(16) glm::vec3 position {0., 0., 0.};
  alignas(16) glm::vec3 scale {1., 1., 1.};
  alignas(16) glm::quat rotation {0., 0., 0., 1.};
};

class entity_component_handle_base {
  public:
  virtual ~entity_component_handle_base() {};
};

template <typename T>
class entity_component_handle : public entity_component_handle_base {
  public:
  entity_component_handle(T pValue) {
    this->mValue = std::make_unique<T>(pValue);
  };
  entity_component_handle(std::unique_ptr<T>&& pValue): mValue(pValue) {};

  virtual ~entity_component_handle() {};

  private:
  std::unique_ptr<T> mValue;
};

class entity {
  public:
  template<class T> std::optional<T> get() {
    std::type_index type_index(typeid(T));
    auto it = this->mComponentMap.find(type_index);
    if (it == this->mComponentMap.end()) {
      return std::nullopt;
    }
    entity_component_handle<T>& handle =
      static_cast<entity_component_handle<T>&>(it->second);
    return *(handle->mValue);
  };
  template<class T> void set(T value) {
    std::type_index type_index(typeid(T));
    entity_component_handle<T> handle(value);
    this->mComponentMap.emplace(std::pair(
      type_index,
      static_cast<entity_component_handle_base>(handle)));
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
  std::map<std::type_index, entity_component_handle_base> mComponentMap;
};
