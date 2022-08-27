#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <map>
#include <typeindex>
#include <any>
#include <cstdint>

struct entity_id {
  uint32_t id;
  uint32_t version;
};

class entity {
  public:
  entity_id mEntityId;
  bool mIsAlive;

  entity():
    mComponentMap(),
    mEntityId(),
    mIsAlive(false) {};
  entity(entity&& pEntity):
    mComponentMap(std::move(pEntity.mComponentMap)),
    mEntityId(pEntity.mEntityId),
    mIsAlive(pEntity.mIsAlive) {};
  entity(const entity& pEntity):
    mComponentMap(pEntity.mComponentMap),
    mEntityId(pEntity.mEntityId),
    mIsAlive(pEntity.mIsAlive) {};

  entity& operator=(entity&& pEntity) {
    mComponentMap = std::move(pEntity.mComponentMap);
    return *this;
  }

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
  template<class T> T& set() {
    T value;
    return this->set<T>(value);
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

#endif // ENTITY_HPP_
