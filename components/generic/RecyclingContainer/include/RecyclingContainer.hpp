#pragma once
#include <cstdint>
#include <deque>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

namespace tamagotchi {

template <typename T, size_t S> class RecyclingContainer {
public:
  /*
     ITERATORS
 */

  class Iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = void;
    using value_type = T;
    using pointer = T *;   // or also value_type*
    using reference = T &; // or also value_type&

    Iterator(typename std::vector<std::unique_ptr<T>>::iterator itr)
        : vecIterator_(std::move(itr)) {}
    reference operator*() const { return *(vecIterator_->get()); }
    pointer operator->() { return vecIterator_->get(); }

    // Prefix increment
    Iterator &operator++() {
      do {
        ++vecIterator_;
      } while (*vecIterator_ == nullptr);
      return *this;
    }

    // Postfix increment
    Iterator operator++(int) {
      Iterator tmp = *this;
      do {
        ++vecIterator_;
      } while (*vecIterator_ == nullptr);
      return tmp;
    }

    // Prefix decrement
    Iterator &operator--() {
      do {
        vecIterator_--;
      } while (*vecIterator_ == nullptr);
      return *this;
    }

    // Postfix decrement
    Iterator operator--(int) {
      Iterator tmp = *this;
      do {
        vecIterator_--;
      } while (*vecIterator_ == nullptr);
      return tmp;
    }

    friend bool operator==(const Iterator &a, const Iterator &b) {
      return a.vecIterator_ == b.vecIterator_;
    };
    friend bool operator!=(const Iterator &a, const Iterator &b) {
      return a.vecIterator_ != b.vecIterator_;
    };

  protected:
    typename std::vector<std::unique_ptr<T>>::iterator vecIterator_;
  };

  class ConstIterator : public Iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = void;
    using value_type = T;
    using pointer = T *;   // or also value_type*
    using reference = T &; // or also value_type&

    const reference operator*() const override {
      return *(Iterator::vecIterator_->get());
    }
  };

  /*
      RECYCLING CONTAINER METHODS
  */

  RecyclingContainer() : maxSize_(S){};
  ~RecyclingContainer() = default;

  bool empty() const { return (items_.size() - freeSlots_.size() == 0); }
  bool full() const { return size() == maxSize_; }
  size_t size() const { return items_.size() - freeSlots_.size(); }
  size_t maxSize() const { return maxSize_; }
  int64_t insert(T toAdd) {
    auto freeSpace = getFreeSpace();
    if (freeSpace == items_.end()) {
      if (items_.size() == maxSize_) {
        return -1;
      }
      items_.emplace_back(std::make_unique<T>(std::move(toAdd)));
      return items_.size() - 1;
    }
    *freeSpace = std::make_unique<T>(std::move(toAdd));
    return std::distance(items_.begin(), freeSpace);
  }
  bool remove(uint64_t elemPosition) {
    if (elemPosition < items_.size() && items_[elemPosition]) {
      items_[elemPosition] = {};
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
private:
  size_t maxSize_;

  typename std::vector<std::unique_ptr<T>> items_;
  typename std::deque<uint64_t> freeSlots_;

  typename std::vector<std::unique_ptr<T>>::iterator getFreeSpace() {
    if (!freeSlots_.empty()) {
      const auto result = items_.begin() + freeSlots_.front();
      freeSlots_.pop_front();
      return result;
    }
    return items_.end();
  }

  friend bool operator==(RecyclingContainer const &lhs,
                         RecyclingContainer const &rhs) {
    return lhs.deviceHandles == rhs.deviceHandles &&
           lhs.freeSlots_ == rhs.freeSlots_;
  }
  friend bool operator!=(RecyclingContainer const &lhs,
                         RecyclingContainer const &rhs) {
    return !(rhs == lhs);
  }
};

} // namespace tamagotchi