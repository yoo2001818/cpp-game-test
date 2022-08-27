#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <vector>
#include <memory>
#include <stack>
#include <cstdint>
#include <iterator>
#include <cstddef>
#include "entity.hpp"

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
        if ((*mIterator)->mIsAlive) return;
        mIterator ++;
      }
    }
  };
  std::vector<std::shared_ptr<entity>> mEntityList;

  std::shared_ptr<entity> create();
  std::shared_ptr<entity> add(entity&& entity);
  void remove(const entity& entity);
  void remove(const entity_id& entity);
  std::shared_ptr<entity> get(const entity_id& id);
  iterator begin();
  iterator end();

  private:
  std::stack<std::shared_ptr<entity>> mDeadEntityList;
  uint32_t mEntityCount;
};

#endif // WORLD_HPP_
