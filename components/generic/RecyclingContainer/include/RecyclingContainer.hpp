#pragma once
#include <stdint.h>

#include <deque>
#include <limits>
#include <memory>
#include <vector>

namespace tamagotchi {

template <typename T, size_t S>
class RecyclingContainer {
 public:
  /*
     ITERATORS
 */

  class Iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = void;
    using value_type = T;
    using pointer = T*;    // or also value_type*
    using reference = T&;  // or also value_type&

    Iterator(typename std::vector<std::unique_ptr<T>>::iterator itr)
        : vecIterator(std::move(itr)) {}
    reference operator*() const { return *(vecIterator->get()); }
    pointer operator->() { return vecIterator->get(); }

    // Prefix increment
    Iterator& operator++() {
      while ((vecIterator++)->get() == nullptr) {
      };
      return *this;
    }

    // Postfix increment
    Iterator operator++(int) {
      Iterator tmp = *this;
      while (++(*this) == nullptr) {
      }
      return tmp;
    }

    // Prefix decrement
    Iterator& operator--() {
      while ((vecIterator--)->get() == nullptr) {
      };
      return *this;
    }

    // Postfix decrement
    Iterator operator--(int) {
      Iterator tmp = *this;
      while (--(*this) == nullptr) {
      }
      return tmp;
    }

    friend bool operator==(const Iterator& a, const Iterator& b) {
      return a.vecIterator == b.vecIterator;
    };
    friend bool operator!=(const Iterator& a, const Iterator& b) {
      return a.vecIterator != b.vecIterator;
    };

   protected:
    typename std::vector<std::unique_ptr<T>>::iterator vecIterator;
  };

  class ConstIterator : public Iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = void;
    using value_type = T;
    using pointer = T*;    // or also value_type*
    using reference = T&;  // or also value_type&

    const reference operator*() const override {
      return *(Iterator::vecIterator->get());
    }
  };

  /*
      RECYCLING CONTAINER METHODS
  */

  RecyclingContainer() : maxSize_(S), counter_(0){};
  ~RecyclingContainer() = default;

  bool empty() const { return (items_.size() - freeSlots_.size() == 0); }
  bool full() const { return counter_ == maxSize_ && freeSlots_.empty(); }
  size_t size() const { return items_.size() - freeSlots_.size(); }
  size_t getCounter() const { return counter_; }
  size_t max_size() const { return maxSize_; }
  int64_t insert(T toAdd) {
    auto freeSpace = get_free_place();
    if (freeSpace != items_.end()) {
      items_.insert(freeSpace, std::make_unique<T>(toAdd));
      return std::distance(items_.begin, freeSpace);
    }
    return -1;
  }
  bool remove(uint64_t elemPosition) {
    if (elemPosition < items_.size() && items_[elemPosition]) {
      items_.erase(items_.begin() + elemPosition);
      freeSlots_.push_back(elemPosition);
      return true;
    }
    return false;
  }
  void clear() {
    items_.clear(items_.begin(), items_.end());
    freeSlots_.clear();
  }

  Iterator begin() { return Iterator(items_.begin()); }
  Iterator end() { return Iterator(items_.end()); }
  ConstIterator cbegin() { return ConstIterator(items_.begin())++; }
  ConstIterator cend() { return ConstIterator(items_.end()); }
  Iterator rbegin() { return end(); }
  Iterator rend() { return begin(); }
  ConstIterator crbegin() { return cend(); }
  ConstIterator crend() { return cbegin(); }

 private:
  size_t maxSize_;
  size_t counter_;

  typename std::vector<std::unique_ptr<T>> items_;
  typename std::deque<uint64_t> freeSlots_;

  typename std::vector<std::unique_ptr<T>>::iterator get_free_place() {
    if (freeSlots_.empty() && counter_ < maxSize_) {
      return items_.begin() + (counter_++);
    } else if (!freeSlots_.empty()) {
      return items_.begin() + freeSlots_.pop_front();
    }
    return items_.end();
  }

  friend bool operator==(RecyclingContainer const& lhs,
                         RecyclingContainer const& rhs) {
    return lhs.deviceHandles == rhs.deviceHandles &&
           lhs.freeSlots_ == rhs.freeSlots_ && lhs.counter_ == rhs.counter_;
  }
  friend bool operator!=(RecyclingContainer const& lhs,
                         RecyclingContainer const& rhs) {
    return !(rhs == lhs);
  }
};

}  // namespace tamagotchi