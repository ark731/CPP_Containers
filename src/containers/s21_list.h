#ifndef S21_LIST_H
#define S21_LIST_H

// #include <cstddef>
// #include <initializer_list>
// #include <iterator>
// #include <stdexcept>
#include <utility>

namespace s21 {

template <typename T>
class List {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class Iterator;
  class ConstIterator;

  List();
  explicit List(size_type count);
  List(std::initializer_list<value_type> init);
  List(const List& other);
  List(List&& other) noexcept;
  ~List();

  List& operator=(const List& other);
  List& operator=(List&& other) noexcept;

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  bool empty() const noexcept;
  size_type size() const noexcept;

  void clear();
  Iterator insert(ConstIterator pos, const value_type& value);
  Iterator insert(ConstIterator pos, value_type&& value);

  Iterator erase(ConstIterator pos);
  void push_back(const value_type& value);
  void push_front(const value_type& value);
  void push_back(value_type&& value);
  void push_front(value_type&& value);
  void pop_back();
  void pop_front();
  void swap(List& other) noexcept;

  template <typename... Args>
  Iterator insert_many(ConstIterator pos, Args&&... args);
  template <typename... Args>
  void insert_many_back(Args&&... args);
  template <typename... Args>
  void insert_many_front(Args&&... args);

  Iterator begin() noexcept;
  ConstIterator begin() const noexcept;
  Iterator end() noexcept;
  ConstIterator end() const noexcept;

  ConstIterator cbegin() const noexcept;
  ConstIterator cend() const noexcept;

 private:
  struct Node {
    value_type data;
    Node* prev;
    Node* next;

    Node(const value_type& value) : data(value), prev(nullptr), next(nullptr) {}
    Node(value_type&& value)
        : data(std::move(value)), prev(nullptr), next(nullptr) {}
  };

  Node* head_;
  Node* tail_;
  size_type size_;

  void copy_from(const List& other);
  void move_from(List&& other);
  void init();
};

template <typename T>
class List<T>::Iterator {
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using reference = T&;

  Iterator() : node_(nullptr), list_(nullptr) {}
  Iterator(Node* node, List<T>* list) : node_(node), list_(list) {}
  Iterator(const Iterator& other) : node_(other.node_), list_(other.list_) {}

  Iterator& operator=(const Iterator& other) {
    node_ = other.node_;
    list_ = other.list_;
    return *this;
  }

  reference operator*() const {
    if (!node_) throw std::out_of_range("Iterator out of range");
    return node_->data;
  }
  pointer operator->() const {
    if (!node_) throw std::out_of_range("Iterator out of range");
    return &(node_->data);
  }

  Iterator& operator++() {
    if (node_) {
      node_ = node_->next;
    }
    return *this;
  }

  Iterator operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
  }

  Iterator& operator--() {
    if (node_) {
      node_ = node_->prev;
    } else {
      node_ = list_->tail_;
    }
    return *this;
  }

  Iterator operator--(int) {
    Iterator temp = *this;
    --(*this);
    return temp;
  }

  bool operator==(const Iterator& other) const { return node_ == other.node_; }
  bool operator!=(const Iterator& other) const { return node_ != other.node_; }

 private:
  Node* node_;
  List<T>* list_;
  friend class List<T>;
};

template <typename T>
class List<T>::ConstIterator {
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = const T*;
  using reference = const T&;

  ConstIterator() : node_(nullptr), list_(nullptr) {}
  ConstIterator(Node* node, const List<T>* list) : node_(node), list_(list) {}
  ConstIterator(const ConstIterator& other)
      : node_(other.node_), list_(other.list_) {}
  ConstIterator(const Iterator& other)
      : node_(other.node_), list_(other.list_) {}

  ConstIterator& operator=(const ConstIterator& other) {
    node_ = other.node_;
    list_ = other.list_;
    return *this;
  }

  reference operator*() const {
    if (!node_) throw std::out_of_range("Iterator out of range");
    return node_->data;
  }
  pointer operator->() const {
    if (!node_) throw std::out_of_range("Iterator out of range");
    return &(node_->data);
  }

  ConstIterator& operator++() {
    if (node_) {
      node_ = node_->next;
    }
    return *this;
  }

  ConstIterator operator++(int) {
    ConstIterator temp = *this;
    ++(*this);
    return temp;
  }

  ConstIterator& operator--() {
    if (node_) {
      node_ = node_->prev;
    } else {
      node_ = list_->tail_;
    }
    return *this;
  }

  ConstIterator operator--(int) {
    ConstIterator temp = *this;
    --(*this);
    return temp;
  }

  bool operator==(const ConstIterator& other) const {
    return node_ == other.node_;
  }
  bool operator!=(const ConstIterator& other) const {
    return node_ != other.node_;
  }

 private:
  Node* node_;
  const List<T>* list_;
  friend class List<T>;
};

template <typename T>
void List<T>::init() {
  head_ = nullptr;
  tail_ = nullptr;
  size_ = 0;
}

template <typename T>
void List<T>::copy_from(const List& other) {
  for (Node* current = other.head_; current != nullptr;
       current = current->next) {
    push_back(current->data);
  }
}

template <typename T>
void List<T>::move_from(List&& other) {
  head_ = other.head_;
  tail_ = other.tail_;
  size_ = other.size_;
  other.init();
}

template <typename T>
List<T>::List() {
  init();
}

template <typename T>
List<T>::List(size_type count) {
  init();
  for (size_type i = 0; i < count; ++i) {
    push_back(value_type{});
  }
}

template <typename T>
List<T>::List(std::initializer_list<value_type> init_list) {
  init();
  for (const auto& value : init_list) {
    push_back(value);
  }
}

template <typename T>
List<T>::List(const List& other) {
  init();
  copy_from(other);
}

template <typename T>
List<T>::List(List&& other) noexcept {
  init();
  move_from(std::move(other));
}

template <typename T>
List<T>::~List() {
  clear();
}

template <typename T>
List<T>& List<T>::operator=(const List& other) {
  if (this != &other) {
    clear();
    copy_from(other);
  }
  return *this;
}

template <typename T>
List<T>& List<T>::operator=(List&& other) noexcept {
  if (this != &other) {
    clear();
    move_from(std::move(other));
  }
  return *this;
}

template <typename T>
typename List<T>::reference List<T>::front() {
  if (empty()) {
    throw std::out_of_range("List is empty");
  }
  return head_->data;
}

template <typename T>
typename List<T>::const_reference List<T>::front() const {
  if (empty()) {
    throw std::out_of_range("List is empty");
  }
  return head_->data;
}

template <typename T>
typename List<T>::reference List<T>::back() {
  if (empty()) {
    throw std::out_of_range("List is empty");
  }
  return tail_->data;
}

template <typename T>
typename List<T>::const_reference List<T>::back() const {
  if (empty()) {
    throw std::out_of_range("List is empty");
  }
  return tail_->data;
}

template <typename T>
bool List<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T>
typename List<T>::size_type List<T>::size() const noexcept {
  return size_;
}

template <typename T>
void List<T>::clear() {
  Node* current = head_;
  while (current != nullptr) {
    Node* next_node = current->next;
    delete current;
    current = next_node;
  }
  init();
}

// For lvalue references
template <typename T>
typename List<T>::Iterator List<T>::insert(ConstIterator pos,
                                           const value_type& value) {
  Node* current = pos.node_;

  if (current == head_) {
    // Insert at the beginning
    Node* new_node = new Node(value);  // Copy constructor
    new_node->next = head_;
    if (head_) head_->prev = new_node;
    head_ = new_node;
    if (!tail_) tail_ = new_node;
    ++size_;
    return Iterator(new_node, this);
  } else if (current == nullptr) {
    // Insert at the end
    push_back(value);
    return Iterator(tail_, this);
  } else {
    // Insert in the middle
    Node* new_node = new Node(value);  // Copy constructor
    new_node->next = current;
    new_node->prev = current->prev;
    current->prev->next = new_node;
    current->prev = new_node;
    ++size_;
    return Iterator(new_node, this);
  }
}

// For rvalue references (as corrected above)

template <typename T>
typename List<T>::Iterator List<T>::insert(ConstIterator pos,
                                           value_type&& value) {
  Node* current = pos.node_;

  if (current == head_) {
    // Insert at the beginning
    Node* new_node = new Node(std::move(value));  // Move constructor
    new_node->next = head_;
    if (head_) head_->prev = new_node;
    head_ = new_node;
    if (!tail_) tail_ = new_node;
    ++size_;
    return Iterator(new_node, this);
  } else if (current == nullptr) {
    // Insert at the end
    push_back(std::move(value));  // Move value directly into push_back
    return Iterator(tail_, this);
  } else {
    // Insert in the middle
    Node* new_node = new Node(std::move(value));  // Move constructor
    new_node->next = current;
    new_node->prev = current->prev;
    current->prev->next = new_node;
    current->prev = new_node;
    ++size_;
    return Iterator(new_node, this);
  }
}

template <typename T>
typename List<T>::Iterator List<T>::erase(ConstIterator pos) {
  Node* current = pos.node_;
  if (!current) {
    throw std::out_of_range("Iterator out of range");
  }

  Node* next_node = current->next;

  if (current->prev) {
    current->prev->next = current->next;
  } else {
    head_ = current->next;
  }

  if (current->next) {
    current->next->prev = current->prev;
  } else {
    tail_ = current->prev;
  }

  delete current;
  --size_;
  return Iterator(next_node, this);
}

// For lvalue references
template <typename T>
void List<T>::push_back(const value_type& value) {
  Node* new_node = new Node(value);  // Copy constructor
  if (tail_) {
    tail_->next = new_node;
    new_node->prev = tail_;
    tail_ = new_node;
  } else {
    head_ = tail_ = new_node;
  }
  ++size_;
}

// For rvalue references
template <typename T>
void List<T>::push_back(value_type&& value) {
  Node* new_node = new Node(std::move(value));  // Move constructor
  if (tail_) {
    tail_->next = new_node;
    new_node->prev = tail_;
    tail_ = new_node;
  } else {
    head_ = tail_ = new_node;
  }
  ++size_;
}

template <typename T>
void List<T>::push_front(const value_type& value) {
  Node* new_node = new Node(value);
  if (head_) {
    head_->prev = new_node;
    new_node->next = head_;
    head_ = new_node;
  } else {
    head_ = tail_ = new_node;
  }
  ++size_;
}

template <typename T>
void List<T>::push_front(value_type&& value) {
  Node* new_node = new Node(std::move(value));
  if (head_) {
    head_->prev = new_node;
    new_node->next = head_;
    head_ = new_node;
  } else {
    head_ = tail_ = new_node;
  }
  ++size_;
}

template <typename T>
void List<T>::pop_back() {
  if (empty()) return;
  Node* old_tail = tail_;
  tail_ = tail_->prev;
  if (tail_) {
    tail_->next = nullptr;
  } else {
    head_ = nullptr;
  }
  delete old_tail;
  --size_;
}

template <typename T>
void List<T>::pop_front() {
  if (empty()) return;
  Node* old_head = head_;
  head_ = head_->next;
  if (head_) {
    head_->prev = nullptr;
  } else {
    tail_ = nullptr;
  }
  delete old_head;
  --size_;
}

template <typename T>
void List<T>::swap(List& other) noexcept {
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
}

template <typename T>
template <typename... Args>
typename List<T>::Iterator List<T>::insert_many(ConstIterator pos,
                                                Args&&... args) {
  Iterator last_inserted;
  ((last_inserted = insert(pos, std::forward<Args>(args))), ...);
  return last_inserted;
}

template <typename T>
template <typename... Args>
void List<T>::insert_many_back(Args&&... args) {
  (push_back(std::forward<Args>(args)), ...);
}

template <typename Tuple, std::size_t... Is>
auto reverse_tuple_impl(Tuple&& tuple, std::index_sequence<Is...>) {
  return std::make_tuple(
      std::get<sizeof...(Is) - 1 - Is>(std::forward<Tuple>(tuple))...);
}

template <typename Tuple>
auto reverse_tuple(Tuple&& tuple) {
  return reverse_tuple_impl(
      std::forward<Tuple>(tuple),
      std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
}

template <typename T>
template <typename... Args>
void List<T>::insert_many_front(Args&&... args) {
  auto reverse_args = std::make_tuple(std::forward<Args>(args)...);
  std::apply(
      [this](auto&&... reversed_args) {
        (..., push_front(std::forward<decltype(reversed_args)>(reversed_args)));
      },
      reverse_tuple(reverse_args));
}

template <typename T>
typename List<T>::Iterator List<T>::begin() noexcept {
  return Iterator(head_, this);
}

template <typename T>
typename List<T>::ConstIterator List<T>::begin() const noexcept {
  return ConstIterator(head_, this);
}

template <typename T>
typename List<T>::Iterator List<T>::end() noexcept {
  return Iterator(nullptr, this);
}

template <typename T>
typename List<T>::ConstIterator List<T>::end() const noexcept {
  return ConstIterator(nullptr, this);
}

template <typename T>
typename List<T>::ConstIterator List<T>::cbegin() const noexcept {
  return ConstIterator(head_, this);
}

template <typename T>
typename List<T>::ConstIterator List<T>::cend() const noexcept {
  return ConstIterator(nullptr, this);
}

}  // namespace s21

#endif  // S21_LIST_H
