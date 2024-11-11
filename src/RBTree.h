#ifndef S21_RBTREE
#define S21_RBTREE

#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <utility>

namespace s21 {

enum class Color { RED, BLACK };

template <typename T>
struct RBTreeNode {
  T value_;
  RBTreeNode* left_ = nullptr;
  RBTreeNode* right_ = nullptr;
  RBTreeNode* parent_ = nullptr;
  Color color_ = Color::RED;

  RBTreeNode(const T& value = T(), RBTreeNode* parent = nullptr,
             Color color = Color::RED)
      : value_(value),
        left_(nullptr),
        right_(nullptr),
        parent_(parent),
        color_(color) {}

  RBTreeNode(const RBTreeNode&) = delete;
  RBTreeNode& operator=(const RBTreeNode&) = delete;

  bool isRed() const { return color_ == Color::RED; }
  bool isBlack() const { return color_ == Color::BLACK; }
};

template <typename T>
class Iterator {
 public:
  using value_type = T;
  using reference = value_type&;
  using Node = RBTreeNode<value_type>;

  Iterator() = default;
  Iterator(Node* cur, Node* end) : current_node_(cur), end_node_(end) {}
  Iterator(Node* cur) : current_node_(cur), end_node_(nullptr) {}
  Iterator(const Iterator& other) = default;
  Iterator(Iterator&& other) noexcept = default;

  Node* getCurNode() const { return current_node_; }
  bool isValid() const { return current_node_ != nullptr; }
  ConstIterator<T> toConstIterator() const {
    return ConstIterator<T>(current_node_, end_node_);
  }

  Iterator& operator=(const Iterator& other) = default;
  Iterator& operator=(Iterator&& other) noexcept = default;
  value_type* operator->() {
    if (!current_node_) {
      throw std::runtime_error("Dereferencing a null iterator");
    }
    return &(current_node_->value_);
  }
  reference operator*() {
    if (!current_node_) {
      throw std::runtime_error("Dereferencing a null iterator");
    }
    return current_node_->value_;
  }
  bool operator==(const Iterator& other) const {
    return current_node_ == other.current_node_;
  }
  bool operator!=(const Iterator& other) const {
    return current_node_ != other.current_node_;
  }
  Iterator& operator++() {
    if (!current_node_) return *this;
    if (current_node_->right_) {
      current_node_ = current_node_->right_;
      while (current_node_->left_) {
        current_node_ = current_node_->left_;
      }
    } else {
      Node* parent = current_node_->parent_;
      while (parent && current_node_ == parent->right_) {
        current_node_ = parent;
        parent = parent->parent_;
      }
      current_node_ = parent;
    }
    return *this;
  }
  Iterator& operator--() {
    if (!current_node_) {
      current_node_ = end_node_;
      return *this;
    }
    if (current_node_->left_) {
      current_node_ = current_node_->left_;
      while (current_node_->right_) {
        current_node_ = current_node_->right_;
      }
    } else {
      Node* parent = current_node_->parent_;
      while (parent && current_node_ == parent->left_) {
        current_node_ = parent;
        parent = parent->parent_;
      }
      current_node_ = parent;
    }
    return *this;
  }
  Iterator operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
  }
  Iterator operator--(int) {
    Iterator temp = *this;
    --(*this);
    return temp;
  }

 protected:
  Node* current_node_ = nullptr;
  Node* end_node_ = nullptr;
};

template <typename T>
class ConstIterator : public Iterator<T> {
 public:
  using value_type = T;
  using const_reference = const value_type&;
  using Node = RBTreeNode<value_type>;

  ConstIterator() = default;
  ConstIterator(Node* cur, Node* end) : Iterator<T>(cur, end) {}
  ConstIterator(Node* cur) : Iterator<T>(cur, nullptr) {}
  ConstIterator(const Iterator<T>& other) : Iterator<T>(other) {}
  ConstIterator(const ConstIterator& other) = default;
  ConstIterator(ConstIterator&& other) noexcept = default;

  ConstIterator& operator=(const ConstIterator& other) = default;
  ConstIterator& operator=(ConstIterator&& other) noexcept = default;

  const value_type* operator->() const {
    return &(this->current_node_->value_);
  }
  const_reference operator*() const { return this->current_node_->value_; }
  ConstIterator next() const {
    ConstIterator temp = *this;
    ++temp;
    return temp;
  }
  ConstIterator prev() const {
    ConstIterator temp = *this;
    --temp;
    return temp;
  }
};

template <typename T, typename Comparator = std::less<T>>
class RBTree {
 public:
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = Iterator<T>;
  using const_iterator = ConstIterator<T>;
  using size_type = std::size_t;
  using Node = RBTreeNode<T>;

  RBTree() : root_(nullptr), endNode_(new Node()), size_(0), comp_() {}
  RBTree(const RBTree& other);
  RBTree(RBTree&& other) noexcept;
  ~RBTree() {
    clear();
    delete endNode_;
  }

  RBTree& operator=(const RBTree& other);
  RBTree& operator=(RBTree&& other) noexcept;

  void leftRotate(Node* node);
  void rightRotate(Node* node);
  void fixInsert(Node* node);
  void fixErase(Node* node);

  void printTree();

  Node* find_min(Node* root) const;
  Node* find_max(Node* root) const;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  bool empty() const { return size_ == 0; }
  size_type size() const { return size_; }
  size_type max_size() const { return std::numeric_limits<size_type>::max(); }
  void printTree(Node* node);
  void clear() {
    deleteTree(root_);
    root_ = nullptr;
    size_ = 0;
  }

  std::pair<iterator, bool> insert(const value_type& value);
  std::pair<iterator, bool> insertNonUniq(const value_type& value);

  void erase(iterator pos);
  void swap(RBTree& other);
  void merge(RBTree& other);
  void mergeNonUniq(RBTree& other);
  iterator find(const_reference key);
  iterator lower_bound(const_reference key);
  iterator upper_bound(const_reference key);
  bool contains(const_reference key);

 private:
  void SwapNode(Node* fNode, Node* sNode);
  Node* copyTree(Node* otherNode, Node* parentNode);
  void deleteTree(Node* node) {
    if (node) {
      deleteTree(node->left_);
      deleteTree(node->right_);
      delete node;
    }
  }

  Node* root_;
  Node* endNode_;
  size_type size_ = 0U;
  Comparator comp_;
};

template <typename T, typename Comparator>
std::pair<typename RBTree<T, Comparator>::iterator, bool>
RBTree<T, Comparator>::insert(const value_type& value) {
  // Case 1: If the tree is empty, insert the first node
  if (root_ == nullptr) {
    root_ = new Node(value);
    root_->color_ = Color::BLACK;  // The first node should be black
    size_++;
    return std::make_pair(iterator(root_, endNode_), true);
  }

  // Case 2: If the value already exists (for set), return an iterator to the
  // existing element
  iterator it = find(value);           // Find the node in the tree
  if (it != end()) {                   // Value already exists
    return std::make_pair(it, false);  // Duplicate, don't insert
  }

  // Case 3: Normal insertion logic (if the value doesn't already exist)
  Node* newNode = new Node(value);
  Node* parent = nullptr;
  Node* current = root_;

  // Find the correct position for the new node
  while (current != nullptr) {
    parent = current;
    if (comp_(value, current->value_)) {
      current = current->left_;
    } else {
      current = current->right_;
    }
  }

  // Insert the new node
  newNode->parent_ = parent;
  if (comp_(value, parent->value_)) {
    parent->left_ = newNode;
  } else {
    parent->right_ = newNode;
  }

  // Fix the Red-Black Tree properties (coloring and rotations)
  fixInsert(newNode);

  size_++;
  return std::make_pair(iterator(newNode, endNode_), true);
}

}  // namespace s21

#endif  // S21_RBTREE
