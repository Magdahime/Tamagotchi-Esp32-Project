#include <stdint.h>

#include <limits>
#include <memory>
#include <queue>
#include <type_traits>
#include <vector>

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "freertos/semphr.h"
#include "hal/spi_types.h"

namespace tamagotchi {

template <typename T, typename S>
class RecyclingContainer<
    T, S, typename std::enable_if<std::is_integral<T>::value>::type> {
 public:
  RecyclingContainer();
  ~RecyclingContainer() = default;

  bool empty() const { return (items.size() - freeSlots.size() == 0); }
  bool full() const {
    return counter == std::numeric_limits<S>::max() && freeSlots.empty();
  }
  size_t size() const { return items.size() - freeSlots.size(); }
  size_t max_size() const { return std::numeric_limits<S>::max(); }

  std::vector<T>::iterator begin();
  std::vector<T>::iterator end();
  std::vector<T>::const_iterator cbegin();
  std::vector<T>::const_iterator cend();
  std::vector<T>::iterator rbegin();
  std::vector<T>::iterator rend();
  std::vector<T>::const_iterator crbegin();
  std::vector<T>::const_iterator crend();

  std::vector<std::unique_ptr<T>>::iterator insert(T toAdd) {
    auto newDescriptor = get_free_place();
    if (newDescriptor != items.end())
      items.insert(newDescriptor, std::make_unique<T>(toAdd));
    return newDescriptor;
  }
  esp_err_t remove(S elemPosition) {
    items.erase(items.begin() + elemPosition);
    freeSlots.push(elemPosition);
  }

  void clear() {
    items.clear(items.begin(), items.end());
    freeSlots.clear();
  }

 private:
  S counter;
  std::vector<std::unique_ptr<T>> items;
  std::queue<S> freeSlots;

  std::vector<std::unique_ptr<T>>::iterator get_free_place() {
    if (freeSlots.empty() && counter < std::numeric_limits<S>::max()) {
      return items.begin() + (counter++);
    } else if (!freeSlots.empty()) {
      return items.begin() + freeSlots.pop();
    }
    return items.end();
  }

  friend bool operator==(RecyclingContainer const &lhs,
                         RecyclingContainer const &rhs) {
    return lhs.deviceHandles == rhs.deviceHandles &&
           lhs.freeSlots == rhs.freeSlots && lhs.counter == rhs.counter;
  };
  friend bool operator!=(RecyclingContainer const &lhs,
                         RecyclingContainer const &rhs) {
    return !(rhs == lhs);
  };
};
}  // namespace tamagotchi