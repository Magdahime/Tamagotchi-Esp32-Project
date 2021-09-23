#pragma once
#include <stdint.h>

#include <deque>
#include <limits>
#include <memory>
#include <vector>

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "freertos/semphr.h"
#include "hal/spi_types.h"

namespace tamagotchi {

template <typename T, typename S,
          typename std::enable_if<std::is_integral<S>::value, S>::type* =
              nullptr>
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

    const reference operator*() const override { return *(Iterator::vecIterator->get()); }
  };

  /*
      RECYCLING CONTAINER METHODS
  */

  RecyclingContainer();
  ~RecyclingContainer() = default;

  bool empty() const { return (items.size() - freeSlots.size() == 0); }
  bool full() const {
    return counter == std::numeric_limits<S>::max() && freeSlots.empty();
  }
  size_t size() const { return items.size() - freeSlots.size(); }
  size_t max_size() const { return std::numeric_limits<S>::max(); }
  typename std::vector<std::unique_ptr<T>>::iterator insert(T toAdd) {
    auto freeSpace = get_free_place();
    if (freeSpace != items.end())
      items.insert(freeSpace, std::make_unique<T>(toAdd));
    return freeSpace;
  }
  bool remove(S elemPosition) {
    if (elemPosition < items.size() && items[elemPosition]) {
      items.erase(items.begin() + elemPosition);
      freeSlots.push(elemPosition);
      return true;
    }
    return false;
  }
  void clear() {
    items.clear(items.begin(), items.end());
    freeSlots.clear();
  }

  Iterator begin() { return Iterator(items.begin()); }
  Iterator end() { return Iterator(items.end()); }
  ConstIterator cbegin() { return ConstIterator(items.begin())++; }
  ConstIterator cend() { return ConstIterator(items.end()); }
  Iterator rbegin() { return end(); }
  Iterator rend() { return begin(); }
  ConstIterator crbegin() { return cend(); }
  ConstIterator crend() { return cbegin(); }

 private:
  S counter;
  typename std::vector<std::unique_ptr<T>> items;
  typename std::deque<S> freeSlots;

  typename std::vector<std::unique_ptr<T>>::iterator get_free_place() {
    if (freeSlots.empty() && counter < std::numeric_limits<S>::max()) {
      return items.begin() + (counter++);
    } else if (!freeSlots.empty()) {
      return items.begin() + freeSlots.pop();
    }
    return items.end();
  }

  friend bool operator==(RecyclingContainer const& lhs,
                         RecyclingContainer const& rhs) {
    return lhs.deviceHandles == rhs.deviceHandles &&
           lhs.freeSlots == rhs.freeSlots && lhs.counter == rhs.counter;
  }
  friend bool operator!=(RecyclingContainer const& lhs,
                         RecyclingContainer const& rhs) {
    return !(rhs == lhs);
  }
};

}  // namespace tamagotchi