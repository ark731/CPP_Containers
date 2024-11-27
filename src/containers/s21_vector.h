#ifndef S21_VECTOR_H
#define S21_VECTOR_H

#include <utility>
// #include <cstddef>
// #include <stdexcept>
// #include <initializer_list>
// #include <iterator>
// #include <algorithm>

namespace s21 {

template <typename T>
class Vector {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;

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
    friend class Vector<T>;
  };

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
    friend class Vector<T>;
  };

  Vector();
  explicit Vector(size_type count);
  Vector(std::initializer_list<value_type> init);
  Vector(const Vector& other);
  Vector(Vector&& other) noexcept;
  ~Vector();

  Vector& operator=(const Vector& other);
  Vector& operator=(Vector&& other) noexcept;

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

  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type capacity() const noexcept;
  void resize(size_type count);
  void resize(size_type count, const value_type& value);
  void reserve(size_type new_capacity);
  void shrink_to_fit();

  void clear() noexcept;
  void push_back(const value_type& value);
  void push_back(value_type&& value);
  void pop_back();
  Iterator insert(ConstIterator pos, const value_type& value);
  Iterator insert(ConstIterator pos, value_type&& value);
  Iterator erase(ConstIterator pos);
  void swap(Vector& other) noexcept;

  template <typename... Args>
  Iterator insert_many(ConstIterator pos, Args&&... args);

  Iterator begin() { return Iterator(data_); }
  ConstIterator begin() const { return ConstIterator(data_); }
  Iterator end() { return Iterator(data_ + size_); }
  ConstIterator end() const { return ConstIterator(data_ + size_); }

  ConstIterator cbegin() const noexcept { return ConstIterator(data_); }
  ConstIterator cend() const noexcept { return ConstIterator(data_ + size_); }

 private:
  pointer data_;
  size_type size_;
  size_type capacity_;

  void reallocate(size_type new_capacity);
};

template <typename T>
Vector<T>::Vector() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
Vector<T>::Vector(size_type count)
    : data_(nullptr), size_(count), capacity_(count) {
  data_ = new value_type[capacity_]{};
}

template <typename T>
Vector<T>::Vector(std::initializer_list<value_type> init)
    : data_(nullptr), size_(init.size()), capacity_(init.size()) {
  data_ = new value_type[capacity_];
  std::copy(init.begin(), init.end(), data_);
}

template <typename T>
Vector<T>::Vector(const Vector& other)
    : data_(nullptr), size_(other.size_), capacity_(other.capacity_) {
  data_ = new value_type[capacity_];
  std::copy(other.data_, other.data_ + size_, data_);
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
  other.data_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

template <typename T>
Vector<T>::~Vector() {
  delete[] data_;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
  if (this != &other) {
    delete[] data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = new value_type[capacity_];
    std::copy(other.data_, other.data_ + size_, data_);
  }
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
  if (this != &other) {
    delete[] data_;
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  return *this;
}

template <typename T>
typename Vector<T>::reference Vector<T>::operator[](size_type index) {
  return data_[index];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::operator[](
    size_type index) const {
  return data_[index];
}

template <typename T>
typename Vector<T>::reference Vector<T>::at(size_type index) {
  if (index >= size_) {
    throw std::out_of_range("Vector index out of range");
  }
  return data_[index];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::at(size_type index) const {
  if (index >= size_) {
    throw std::out_of_range("Vector index out of range");
  }
  return data_[index];
}

template <typename T>
typename Vector<T>::reference Vector<T>::front() {
  return data_[0];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::front() const {
  return data_[0];
}

template <typename T>
typename Vector<T>::reference Vector<T>::back() {
  return data_[size_ - 1];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::back() const {
  return data_[size_ - 1];
}

template <typename T>
bool Vector<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::size() const noexcept {
  return size_;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::capacity() const noexcept {
  return capacity_;
}

template <typename T>
void Vector<T>::resize(size_type count) {
  if (count < size_) {
    size_ = count;
  } else if (count > size_) {
    if (count > capacity_) {
      reserve(std::max(capacity_ * 2, count));
    }
    for (size_type i = size_; i < count; ++i) {
      data_[i] = value_type{};
    }
    size_ = count;
  }
}

template <typename T>
void Vector<T>::resize(size_type count, const value_type& value) {
  if (count < size_) {
    size_ = count;
  } else if (count > size_) {
    if (count > capacity_) {
      reserve(std::max(capacity_ * 2, count));
    }
    for (size_type i = size_; i < count; ++i) {
      data_[i] = value;
    }
    size_ = count;
  }
}

template <typename T>
void Vector<T>::reserve(size_type new_capacity) {
  if (new_capacity > capacity_) {
    reallocate(new_capacity);
  }
}

template <typename T>
void Vector<T>::shrink_to_fit() {
  if (capacity_ > size_) {
    reallocate(size_);
  }
}

template <typename T>
void Vector<T>::clear() noexcept {
  size_ = 0;
}

template <typename T>
void Vector<T>::push_back(const value_type& value) {
  if (size_ == capacity_) {
    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
  }
  data_[size_++] = value;
}

template <typename T>
void Vector<T>::push_back(value_type&& value) {
  if (size_ == capacity_) {
    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
  }
  data_[size_++] = std::move(value);
}

template <typename T>
void Vector<T>::pop_back() {
  if (size_ > 0) {
    --size_;
  }
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::insert(ConstIterator pos,
                                               const value_type& value) {
  difference_type index = pos.ptr_ - data_;
  if (index < 0 || static_cast<size_type>(index) > size_) {
    throw std::out_of_range("Iterator out of range");
  }
  if (size_ == capacity_) {
    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
  }
  // Shift elements to make space
  for (size_type i = size_; i > static_cast<size_type>(index); --i) {
    data_[i] = std::move(data_[i - 1]);
  }
  // Copy the value into position
  data_[index] = value;
  ++size_;
  return Iterator(data_ + index);
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::insert(ConstIterator pos,
                                               value_type&& value) {
  difference_type index = pos.ptr_ - data_;
  if (index < 0 || static_cast<size_type>(index) > size_) {
    throw std::out_of_range("Iterator out of range");
  }
  if (size_ == capacity_) {
    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
  }
  // Shift elements to make space
  for (size_type i = size_; i > static_cast<size_type>(index); --i) {
    data_[i] = std::move(data_[i - 1]);
  }
  // Move the value into position
  data_[index] = std::move(value);
  ++size_;
  return Iterator(data_ + index);
}

template <typename T>
template <typename... Args>
typename Vector<T>::Iterator Vector<T>::insert_many(ConstIterator pos,
                                                    Args&&... args) {
  difference_type index = pos.ptr_ - data_;
  if (index < 0 || static_cast<size_type>(index) > size_) {
    throw std::out_of_range("Iterator out of range");
  }
  size_type n = sizeof...(Args);
  if (size_ + n > capacity_) {
    size_type new_capacity = std::max(capacity_ * 2, size_ + n);
    pointer new_data = new value_type[new_capacity];
    try {
      for (size_type i = 0; i < static_cast<size_type>(index); ++i) {
        new_data[i] = std::move(data_[i]);
      }
      size_type insert_index = index;
      ((new_data[insert_index++] = std::forward<Args>(args)), ...);
      for (size_type i = static_cast<size_type>(index); i < size_; ++i) {
        new_data[insert_index++] = std::move(data_[i]);
      }
    } catch (...) {
      delete[] new_data;
      throw;
    }

    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  } else {
    try {
      for (size_type i = size_; i > static_cast<size_type>(index); --i) {
        data_[i + n - 1] = std::move(data_[i - 1]);
      }
      size_type insert_index = index;
      ((data_[insert_index++] = std::forward<Args>(args)), ...);
    } catch (...) {
      for (size_type i = static_cast<size_type>(index); i < size_; ++i) {
        data_[i] = std::move(data_[i + n]);
      }
      throw;
    }
  }
  size_ += n;
  return Iterator(data_ + index);
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::erase(ConstIterator pos) {
  difference_type index = pos.ptr_ - data_;
  if (index < 0 || static_cast<size_type>(index) >= size_) {
    throw std::out_of_range("Iterator out of range");
  }
  for (size_type i = static_cast<size_type>(index); i < size_ - 1; ++i) {
    data_[i] = std::move(data_[i + 1]);
  }
  --size_;
  return Iterator(data_ + index);
}

template <typename T>
void Vector<T>::swap(Vector& other) noexcept {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}
template <typename T>
void Vector<T>::reallocate(size_type new_capacity) {
  pointer new_data = new value_type[new_capacity];
  for (size_type i = 0; i < size_; ++i) {
    new_data[i] = std::move(data_[i]);
  }
  delete[] data_;
  data_ = new_data;
  capacity_ = new_capacity;
}

}  // namespace s21

#endif  // S21_VECTOR_H
