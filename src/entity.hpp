#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <map>
#include <typeindex>
#include <any>
#include <cstdint>

struct entity_id {
  uint32_t id;
  uint32_t version;

  friend bool operator== (const entity_id& e1, const entity_id& e2);
  friend bool operator!= (const entity_id& e1, const entity_id& e2);

  friend bool operator< (const entity_id& e1, const entity_id& e2);
  friend bool operator> (const entity_id& e1, const entity_id& e2);

  friend bool operator<= (const entity_id& e1, const entity_id& e2);
  friend bool operator>= (const entity_id& e1, const entity_id& e2);
};

class entity {
  public:
  bool mIsAlive;
  bool mIsDirty;

  entity():
    mComponentMap(),
    mEntityId(),
    mIsAlive(false),
    mIsDirty(false) {};
  entity(entity&& pEntity):
    mComponentMap(std::move(pEntity.mComponentMap)),
    mEntityId(pEntity.mEntityId),
    mIsAlive(pEntity.mIsAlive),
    mIsDirty(pEntity.mIsDirty) {};
  entity(const entity& pEntity):
    mComponentMap(pEntity.mComponentMap),
    mEntityId(pEntity.mEntityId),
    mIsAlive(pEntity.mIsAlive),
    mIsDirty(pEntity.mIsDirty) {};

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
  template<class T> bool has() const {
    std::type_index type_index(typeid(T));
    return this->mComponentMap.find(type_index) != this->mComponentMap.end();
  };
  
  const entity_id& getId() const {
    return this->mEntityId;
  };

  bool isAlive() const {
    return this->mIsAlive;
  }

  bool isDirty() const {
    return this->mIsDirty;
  }

  private:
  std::map<std::type_index, std::any> mComponentMap;
  entity_id mEntityId;

  friend class world;
};

#endif // ENTITY_HPP_
