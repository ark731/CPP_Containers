#ifndef S21_ARRAY_H
#define S21_ARRAY_H

// #include <cstddef>
// #include <initializer_list>
// #include <stdexcept>
#include <utility>

namespace s21 {

template <typename T, std::size_t N>
class Array {
 public:
  // Type definitions
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;

  // Iterator class
  class Iterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    Iterator() : ptr_(nullptr) {}
    Iterator(pointer ptr) : ptr_(ptr) {}
    Iterator(const Iterator& other) : ptr_(other.ptr_) {}

    Iterator& operator=(const Iterator& other) {
      ptr_ = other.ptr_;
      return *this;
    }

    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }

    Iterator& operator++() {
      ++ptr_;
      return *this;
    }

    Iterator operator++(int) {
      Iterator temp = *this;
      ++(*this);
      return temp;
    }

    Iterator& operator--() {
      --ptr_;
      return *this;
    }

    Iterator operator--(int) {
      Iterator temp = *this;
      --(*this);
      return temp;
    }

    Iterator operator+(difference_type n) const { return Iterator(ptr_ + n); }
    Iterator operator-(difference_type n) const { return Iterator(ptr_ - n); }

    difference_type operator-(const Iterator& other) const {
      return ptr_ - other.ptr_;
    }

    Iterator& operator+=(difference_type n) {
      ptr_ += n;
      return *this;
    }

    Iterator& operator-=(difference_type n) {
      ptr_ -= n;
      return *this;
    }

    reference operator[](difference_type n) const { return *(ptr_ + n); }

    bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const Iterator& other) const { return ptr_ != other.ptr_; }
    bool operator<(const Iterator& other) const { return ptr_ < other.ptr_; }
    bool operator<=(const Iterator& other) const { return ptr_ <= other.ptr_; }
    bool operator>(const Iterator& other) const { return ptr_ > other.ptr_; }
    bool operator>=(const Iterator& other) const { return ptr_ >= other.ptr_; }

   private:
    pointer ptr_;
    friend class Array<T, N>;
  };

  // ConstIterator class
  class ConstIterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    ConstIterator() : ptr_(nullptr) {}
    ConstIterator(const_pointer ptr) : ptr_(ptr) {}
    ConstIterator(const ConstIterator& other) : ptr_(other.ptr_) {}
    ConstIterator(const Iterator& other) : ptr_(other.ptr_) {}

    ConstIterator& operator=(const ConstIterator& other) {
      ptr_ = other.ptr_;
      return *this;
    }

    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }

    // Increment and decrement operators
    ConstIterator& operator++() {
      ++ptr_;
      return *this;
    }

    ConstIterator operator++(int) {
      ConstIterator temp = *this;
      ++(*this);
      return temp;
    }

    ConstIterator& operator--() {
      --ptr_;
      return *this;
    }

    ConstIterator operator--(int) {
      ConstIterator temp = *this;
      --(*this);
      return temp;
    }

    ConstIterator operator+(difference_type n) const {
      return ConstIterator(ptr_ + n);
    }
    ConstIterator operator-(difference_type n) const {
      return ConstIterator(ptr_ - n);
    }

    difference_type operator-(const ConstIterator& other) const {
      return ptr_ - other.ptr_;
    }

    ConstIterator& operator+=(difference_type n) {
      ptr_ += n;
      return *this;
    }

    ConstIterator& operator-=(difference_type n) {
      ptr_ -= n;
      return *this;
    }

    reference operator[](difference_type n) const { return *(ptr_ + n); }

    // Comparison operators
    bool operator==(const ConstIterator& other) const {
      return ptr_ == other.ptr_;
    }
    bool operator!=(const ConstIterator& other) const {
      return ptr_ != other.ptr_;
    }
    bool operator<(const ConstIterator& other) const {
      return ptr_ < other.ptr_;
    }
    bool operator<=(const ConstIterator& other) const {
      return ptr_ <= other.ptr_;
    }
    bool operator>(const ConstIterator& other) const {
      return ptr_ > other.ptr_;
    }
    bool operator>=(const ConstIterator& other) const {
      return ptr_ >= other.ptr_;
    }

   private:
    const_pointer ptr_;
    friend class Array<T, N>;
  };

  // Constructors
  Array() = default;
  Array(const Array& other);
  Array(Array&& other) noexcept;
  Array(std::initializer_list<value_type> init);

  // Assignment operators
  Array& operator=(const Array& other);
  Array& operator=(Array&& other) noexcept;

  // Element access
  reference operator[](size_type index);
  const_reference operator[](size_type index) const;

  reference at(size_type index);
  const_reference at(size_type index) const;

  reference front();
  const_reference front() const;

  reference back();
  const_reference back() const;

  pointer data() noexcept { return data_; }
  const_pointer data() const noexcept { return data_; }

  // Capacity
  constexpr bool empty() const noexcept;
  constexpr size_type size() const noexcept;
  constexpr size_type max_size() const noexcept;

  // Modifiers
  void fill(const_reference value);
  void swap(Array& other) noexcept;

  // Iterators
  Iterator begin() { return Iterator(data_); }
  ConstIterator begin() const { return ConstIterator(data_); }
  Iterator end() { return Iterator(data_ + N); }
  ConstIterator end() const { return ConstIterator(data_ + N); }

  ConstIterator cbegin() const noexcept { return ConstIterator(data_); }
  ConstIterator cend() const noexcept { return ConstIterator(data_ + N); }

 private:
  value_type data_[N]{};
};

// Implementation

// Copy constructor
template <typename T, std::size_t N>
Array<T, N>::Array(const Array& other) {
  for (size_type i = 0; i < N; ++i) {
    data_[i] = other.data_[i];
  }
}

// Move constructor
template <typename T, std::size_t N>
Array<T, N>::Array(Array&& other) noexcept {
  for (size_type i = 0; i < N; ++i) {
    data_[i] = std::move(other.data_[i]);
  }
}

// Initializer list constructor
template <typename T, std::size_t N>
Array<T, N>::Array(std::initializer_list<value_type> init) {
  size_type i = 0;
  for (const auto& value : init) {
    if (i >= N) break;
    data_[i++] = value;
  }
}

// Copy assignment operator
template <typename T, std::size_t N>
Array<T, N>& Array<T, N>::operator=(const Array& other) {
  if (this != &other) {
    for (size_type i = 0; i < N; ++i) {
      data_[i] = other.data_[i];
    }
  }
  return *this;
}

// Move assignment operator
template <typename T, std::size_t N>
Array<T, N>& Array<T, N>::operator=(Array&& other) noexcept {
  if (this != &other) {
    for (size_type i = 0; i < N; ++i) {
      data_[i] = std::move(other.data_[i]);
    }
  }
  return *this;
}

// Element access

template <typename T, std::size_t N>
typename Array<T, N>::reference Array<T, N>::operator[](size_type index) {
  return data_[index];
}

template <typename T, std::size_t N>
typename Array<T, N>::const_reference Array<T, N>::operator[](
    size_type index) const {
  return data_[index];
}

template <typename T, std::size_t N>
typename Array<T, N>::reference Array<T, N>::at(size_type index) {
  if (index >= N) {
    throw std::out_of_range("Array index out of range");
  }
  return data_[index];
}

template <typename T, std::size_t N>
typename Array<T, N>::const_reference Array<T, N>::at(size_type index) const {
  if (index >= N) {
    throw std::out_of_range("Array index out of range");
  }
  return data_[index];
}

template <typename T, std::size_t N>
typename Array<T, N>::reference Array<T, N>::front() {
  return data_[0];
}

template <typename T, std::size_t N>
typename Array<T, N>::const_reference Array<T, N>::front() const {
  return data_[0];
}

template <typename T, std::size_t N>
typename Array<T, N>::reference Array<T, N>::back() {
  return data_[N - 1];
}

template <typename T, std::size_t N>
typename Array<T, N>::const_reference Array<T, N>::back() const {
  return data_[N - 1];
}

// Capacity

template <typename T, std::size_t N>
constexpr bool Array<T, N>::empty() const noexcept {
  return N == 0;
}

template <typename T, std::size_t N>
constexpr typename Array<T, N>::size_type Array<T, N>::size() const noexcept {
  return N;
}

template <typename T, std::size_t N>
constexpr typename Array<T, N>::size_type Array<T, N>::max_size()
    const noexcept {
  return N;
}

// Modifiers

template <typename T, std::size_t N>
void Array<T, N>::fill(const_reference value) {
  for (size_type i = 0; i < N; ++i) {
    data_[i] = value;
  }
}

template <typename T, std::size_t N>
void Array<T, N>::swap(Array& other) noexcept {
  for (size_type i = 0; i < N; ++i) {
    std::swap(data_[i], other.data_[i]);
  }
}

}  // namespace s21

#endif  // S21_ARRAY_H
