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

class world {
  public:
  struct iterator {
    public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = std::shared_ptr<entity>;
    using pointer = value_type*;
    using reference = value_type&;

    using base_iterator = std::vector<std::shared_ptr<entity>>::iterator;

    iterator(
      base_iterator pIterator,
      base_iterator pEnd
    ): mIterator(pIterator), mEnd(pEnd) {
      skipToValid();
    };
    iterator(const iterator&) = default;
    ~iterator() {};
    iterator& operator=(const iterator& target) {
      mIterator = target.mIterator;
      return *this;
    };
    iterator& operator++() {
      ++ mIterator; 
      skipToValid();
      return *this;
    };
    iterator operator++(int) {
      iterator copy(*this);
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
    friend void swap(iterator& lhs, iterator& rhs) {
      std::swap(lhs.mIterator, rhs.mIterator);
    };
    friend bool operator==(const iterator& lhs, const iterator& rhs) {
      return lhs.mIterator == rhs.mIterator;
    };
    friend bool operator!=(const iterator& lhs, const iterator& rhs) {
      return lhs.mIterator != rhs.mIterator;
    };

    private:
    std::vector<std::shared_ptr<entity>>::iterator mIterator;
    std::vector<std::shared_ptr<entity>>::iterator mEnd;

    void skipToValid() {
      while (mIterator != mEnd) {
        if ((*mIterator)->isAlive()) return;
        mIterator ++;
      }
    }
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
  iterator begin();
  iterator end();
  void markDirty(const entity& entity);
  void markDirty(const entity_id& id);
  void updateIndex();
  tile_index& getTileIndex();

  private:
  std::stack<std::shared_ptr<entity>> mDeadEntityList;
  std::stack<entity_id> mDirtyEntityList;
  uint32_t mEntityCount;
  tile_index mTileIndex;
};

#endif // WORLD_HPP_
