#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <vector>
#include <memory>
#include <stack>
#include <cstdint>
#include <iterator>
#include <cstddef>
#include "entity.hpp"
#include "tile_index.hpp"

template <class base_t, class filter_t>
struct filtered_iterator {
  public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = std::shared_ptr<entity>;
  using pointer = value_type*;
  using reference = value_type&;

  using base_iterator = base_t;

  filtered_iterator(
    base_iterator pIterator,
    base_iterator pEnd,
    filter_t pFilter
  ): mIterator(pIterator), mEnd(pEnd), mFilter(pFilter) {
    skipToValid();
  };
  filtered_iterator(const filtered_iterator&) = default;
  ~filtered_iterator() {};
  filtered_iterator& operator=(const filtered_iterator& target) {
    mIterator = target.mIterator;
    return *this;
  };
  filtered_iterator& operator++() {
    ++ mIterator; 
    skipToValid();
    return *this;
  };
  filtered_iterator operator++(int) {
    filtered_iterator copy(*this);
    mIterator ++;
    skipToValid();
    return copy;
  };
  reference operator*() const {
    return *mIterator;
  };
  pointer operator->() const {
    return &*mIterator;
  };
  friend void swap(filtered_iterator& lhs, filtered_iterator& rhs) {
    std::swap(lhs.mIterator, rhs.mIterator);
  };
  friend bool operator==(const filtered_iterator& lhs, const filtered_iterator& rhs) {
    return lhs.mIterator == rhs.mIterator;
  };
  friend bool operator!=(const filtered_iterator& lhs, const filtered_iterator& rhs) {
    return lhs.mIterator != rhs.mIterator;
  };

  private:
  base_iterator mIterator;
  base_iterator mEnd;
  filter_t mFilter;

  void skipToValid() {
    while (mIterator != mEnd) {
      if (mFilter.test(**mIterator)) return;
      mIterator++;
    }
  }
};

class world {
  public:
  class entity_filter {
    public:
    bool test(entity& entity) {
      return entity.isAlive();
    };
  };
  using entity_iterator = filtered_iterator<std::vector<std::shared_ptr<entity>>::iterator, entity_filter>;
  template<class T, class... Ts>
  class query {
    public:
    class filter {
      public:
      bool test(entity& entity) {
        return mParent.test(entity) && entity.has<T>();
      };
      query<Ts...>::filter mParent;
    };
    using iterator = filtered_iterator<entity_iterator, filter>;
    query(world * pWorld): mWorld(pWorld) {};
    iterator begin() {
      return iterator(mWorld->begin(), mWorld->end(), {});
    };
    iterator end() {
      return iterator(mWorld->end(), mWorld->end(), {});
    };
    private:
    world * mWorld;
  };
  template<class T>
  class query<T> {
    public:
    class filter {
      public:
      bool test(entity& entity) {
        return entity.has<T>();
      };
    };
    using iterator = filtered_iterator<entity_iterator, filter>;
    query(world * pWorld): mWorld(pWorld) {};
    iterator begin() {
      return iterator(mWorld->begin(), mWorld->end(), {});
    };
    iterator end() {
      return iterator(mWorld->end(), mWorld->end(), {});
    };
    private:
    world * mWorld;
  };
  std::vector<std::shared_ptr<entity>> mEntityList;

  world():
    mEntityList(),
    mDeadEntityList(),
    mDirtyEntityList(),
    mEntityCount(0),
    mTileIndex(*this) {};

  std::shared_ptr<entity> create();
  std::shared_ptr<entity> add(entity&& entity);
  void remove(const entity& entity);
  void remove(const entity_id& entity);
  std::shared_ptr<entity> get(const entity_id& id) const;
  entity_iterator begin();
  entity_iterator end();
  void markDirty(const entity& entity);
  void markDirty(const entity_id& id);
  void updateIndex();
  tile_index& getTileIndex();
  template<class... Ts> query<Ts...> getQuery() {
    return { this };
  };

  private:
  std::stack<std::shared_ptr<entity>> mDeadEntityList;
  std::stack<entity_id> mDirtyEntityList;
  uint32_t mEntityCount;
  tile_index mTileIndex;
};

#endif // WORLD_HPP_
