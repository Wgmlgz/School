#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <utility>

class IntVector {
 public:
  explicit IntVector(size_t size = 0) {
    while (capacity_ < size) doubleCapacity();
    size_ = size;
  }

  explicit IntVector(std::initializer_list<int> list) {
    for (const auto& i : list) {
      PushBack(i);
    }
    realloc(list.size());
  }

  IntVector(const IntVector& other) {
    FreeMem();
    size_ = other.size_;
    capacity_ = other.capacity_;
    head_ = new int[capacity_];
    std::copy_n(other.head_, capacity_, head_); 
  }

  IntVector(IntVector&& other) {
    size_ = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, 0);

    head_ = std::exchange(other.head_, nullptr);
  }

  ~IntVector() { delete[] head_; }

  void Swap(IntVector& other) {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(head_, other.head_);
  }

  IntVector& operator=(const IntVector& other) {
    IntVector temp(other);
    Swap(temp);
    return *this;
  }

  size_t Size() const { return size_; }
  size_t Capacity() const { return capacity_; }

  void PushBack(const int& val) {
    if (size_ == capacity_) doubleCapacity();
    operator[](size_++) = val;
  }

  void PopBack() { size_ = --size_ < 0 ? 0 : size_; }

  void Clear() { size_ = 0; }

  void Reserve(size_t size) {
    if (size > capacity_) realloc(size);
  }

  int operator[](const size_t pos) const { return *(head_ + pos); }
  int& operator[](const size_t pos) { return *(head_ + pos); }

  void FreeMem() {
    delete[] head_;
    size_ = 0;
    capacity_ = 0;
  }

 private:
  size_t size_ = 0;
  size_t capacity_ = 0;
  int* head_ = nullptr;

  void realloc(size_t size) {
    auto new_head = new int[size]();
    size_ = std::min(size, size_);
    std::copy_n(head_, size_, new_head);
    head_ = new_head;
    capacity_ = size;
  }
  
  void doubleCapacity() {
    size_t new_capacity = capacity_ ? capacity_ * 2 : 1;
    realloc(new_capacity);
  }
};
