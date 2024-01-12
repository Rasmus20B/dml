
#include <__expected/expected.h>
#include <expected>
#include <array>
#include <stdexcept>

namespace dml {
namespace ds {

template<typename T, int size>
struct fill_array {
  std::expected<T&, std::overflow_error> emplace(T val) {
    uint32_t i = 0;
    while(i < arr.size()) {
      if(this->mask[i] == false) {
        this->arr[i] = val;
        this->mask = true;
        this->n_elems++;
        return &arr[i];
      }
      ++i;
    }
    return std::overflow_error("Unable to add new element: Array is full");
  }

  void remove(uint32_t idx) {
    mask[idx] = false;
  }

  [[nodiscard]] T& operator[](size_t idx) {
    return arr[idx];
  }

  private:
    std::array<T, size> arr;
    std::array<bool, size> mask;
    uint32_t n_elems = 1;
};

}
}
