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

    iterator(std::vector<std::shared_ptr<entity>>::iterator pEnclosedIterator):
      mEnclosedIterator(pEnclosedIterator) {};
    iterator(const iterator&) = default;
    ~iterator() {};
    iterator& operator=(const iterator& target) {
      mEnclosedIterator = target.mEnclosedIterator;
      return *this;
    };
    iterator& operator++() {
      ++ mEnclosedIterator; 
      return *this;
    };
    iterator operator++(int) {
      return mEnclosedIterator ++;
    };
    reference operator*() const {
      return *mEnclosedIterator;
    };
    pointer operator->() const {
      return &*mEnclosedIterator;
    };
    friend void swap(iterator& lhs, iterator& rhs) {
      std::swap(lhs.mEnclosedIterator, rhs.mEnclosedIterator);
    };
    friend bool operator==(const iterator& lhs, const iterator& rhs) {
      return lhs.mEnclosedIterator == rhs.mEnclosedIterator;
    };
    friend bool operator!=(const iterator& lhs, const iterator& rhs) {
      return lhs.mEnclosedIterator != rhs.mEnclosedIterator;
    };

    private:
    std::vector<std::shared_ptr<entity>>::iterator mEnclosedIterator;
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
