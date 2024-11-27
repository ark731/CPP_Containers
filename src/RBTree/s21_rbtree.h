#ifndef S21_RBTREE
#define S21_RBTREE

#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

namespace s21 {

template <typename T, typename Comparator = std::less<T>>
class RBTree;
template <typename T>
class ConstIterator;

template <typename T>
struct is_pair : std::false_type {};

template <typename T1, typename T2>
struct is_pair<std::pair<T1, T2>> : std::true_type {};

template <typename T>
inline constexpr bool is_pair_v = is_pair<T>::value;

enum class Color { RED, BLACK };

template <typename T>
struct RBTreeNode {
  T value_;
  RBTreeNode* left_ = nullptr;
  RBTreeNode* right_ = nullptr;
  RBTreeNode* parent_ = nullptr;
  Color color_ = Color::RED;

  RBTreeNode(const T& value, RBTreeNode* parent = nullptr,
             Color color = Color::RED)
      : value_(value),
        left_(nullptr),
        right_(nullptr),
        parent_(parent),
        color_(color) {}

  RBTreeNode(RBTreeNode* parent = nullptr)
      : left_(nullptr),
        right_(nullptr),
        parent_(parent),
        color_(Color::BLACK) {}

  RBTreeNode(const RBTreeNode&) = delete;
  RBTreeNode& operator=(const RBTreeNode&) = delete;

  bool isRed() const { return color_ == Color::RED; }
  bool isBlack() const { return color_ == Color::BLACK; }
};

////////////////////////////////////
// --------- ITERATOR ----------- //
////////////////////////////////////

template <typename T>
class Iterator {
 public:
  using value_type = T;
  using reference = value_type&;
  using pointer = value_type*;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;
  using Node = RBTreeNode<value_type>;

  Iterator() = default;
  Iterator(Node* cur, Node* end) : current_node_(cur), end_node_(end) {}
  Iterator(Node* cur) : current_node_(cur), end_node_(nullptr) {}
  Iterator(const Iterator& other) = default;
  Iterator(Iterator&& other) noexcept;

  ConstIterator<T> toConstIterator() const {
    return ConstIterator<T>(current_node_, end_node_);
  }

  Iterator& operator=(const Iterator& other) = default;
  Iterator& operator=(Iterator&& other) noexcept;
  reference operator*();
  value_type* operator->();
  bool operator==(const Iterator& other) const;
  bool operator!=(const Iterator& other) const;
  Iterator& operator++();
  Iterator& operator--();
  Iterator operator++(int);
  Iterator operator--(int);
  Node* getCurNode() const;
  Node* getMaxNode() const;
  bool isValid() const;
  reference getCurrentValue() const;

 protected:
  Node* current_node_ = nullptr;
  Node* end_node_ = nullptr;
};

// --- Implementations for Iterator ---

template <typename T>
Iterator<T>::Iterator(Iterator&& other) noexcept
    : current_node_(other.current_node_), end_node_(other.end_node_) {
  other.current_node_ = other.end_node_;
}

template <typename T>
Iterator<T>& Iterator<T>::operator=(Iterator&& other) noexcept {
  if (this != &other) {
    current_node_ = other.current_node_;
    end_node_ = other.end_node_;
    other.current_node_ = other.end_node_;
  }
  return *this;
}

template <typename T>
typename Iterator<T>::Node* Iterator<T>::getCurNode() const {
  return current_node_;
}

template <typename T>
typename Iterator<T>::Node* Iterator<T>::getMaxNode() const {
  Node* node = current_node_;
  if (node == nullptr) {
    return end_node_;
  }
  if (node == end_node_) {
    return end_node_->parent_;
  }
  while (node->parent_ != nullptr) {
    node = node->parent_;
  }
  while (node->right_ && node->right_ != end_node_ && node->right_ != nullptr) {
    node = node->right_;
  }
  return node;
}

template <typename T>
bool Iterator<T>::isValid() const {
  return current_node_ != nullptr;
}

template <typename T>
typename Iterator<T>::reference Iterator<T>::getCurrentValue() const {
  if (!current_node_) {
    throw std::runtime_error("Dereferencing a null iterator");
  }
  return current_node_->value_;
}

template <typename T>
typename Iterator<T>::reference Iterator<T>::operator*() {
  return getCurrentValue();
}

template <typename T>
typename Iterator<T>::value_type* Iterator<T>::operator->() {
  return &getCurrentValue();
}

template <typename T>
bool Iterator<T>::operator==(const Iterator& other) const {
  return current_node_ == other.current_node_;
}

template <typename T>
bool Iterator<T>::operator!=(const Iterator& other) const {
  return current_node_ != other.current_node_;
}

template <typename T>
Iterator<T>& Iterator<T>::operator++() {
  if (current_node_ == end_node_) return *this;

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
    current_node_ = parent ? parent : end_node_;
  }
  return *this;
}

template <typename T>
Iterator<T>& Iterator<T>::operator--() {
  if (current_node_ == nullptr) {
    current_node_ = end_node_;
  } else if (current_node_ == end_node_) {
    current_node_ = getMaxNode();
  } else if (current_node_->left_) {
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
    current_node_ = parent ? parent : end_node_;
  }
  return *this;
}

template <typename T>
Iterator<T> Iterator<T>::operator++(int) {
  Iterator temp = *this;
  ++(*this);
  return temp;
}

template <typename T>
Iterator<T> Iterator<T>::operator--(int) {
  Iterator temp = *this;
  --(*this);
  return temp;
}

///////////////////////////////////////
// -------- CONST ITERATOR --------- //
///////////////////////////////////////

template <typename T>
class ConstIterator : public Iterator<T> {
 public:
  using value_type = T;
  using const_reference = const value_type&;
  using Node = typename Iterator<T>::Node;

  ConstIterator() = default;
  ConstIterator(Node* cur, Node* end) : Iterator<T>(cur, end) {}
  ConstIterator(Node* cur) : Iterator<T>(cur, nullptr) {}
  ConstIterator(const Iterator<T>& other) : Iterator<T>(other) {}
  ConstIterator(const ConstIterator& other) = default;
  ConstIterator(ConstIterator&& other) noexcept;
  ConstIterator& operator=(const ConstIterator& other) = default;
  ConstIterator& operator=(ConstIterator&& other) noexcept;
  const value_type* operator->() const;
  const_reference operator*() const;
  ConstIterator next() const;
  ConstIterator prev() const;
};

// --- Implementations for Const Iterator ---

template <typename T>
ConstIterator<T>::ConstIterator(ConstIterator&& other) noexcept
    : Iterator<T>(std::move(other)) {
  other.current_node_ = other.end_node_;
}

template <typename T>
ConstIterator<T>& ConstIterator<T>::operator=(ConstIterator&& other) noexcept {
  if (this != &other) {
    Iterator<T>::operator=(std::move(other));
    other.current_node_ = other.end_node_;
  }
  return *this;
}

template <typename T>
const typename ConstIterator<T>::value_type* ConstIterator<T>::operator->()
    const {
  return &(this->current_node_->value_);
}

template <typename T>
typename ConstIterator<T>::const_reference ConstIterator<T>::operator*() const {
  return this->current_node_->value_;
}

template <typename T>
ConstIterator<T> ConstIterator<T>::next() const {
  ConstIterator temp = *this;
  ++temp;
  return temp;
}

template <typename T>
ConstIterator<T> ConstIterator<T>::prev() const {
  ConstIterator temp = *this;
  --temp;
  return temp;
}

///////////////////////////////////////
// -------- RED-BLACK TREE --------- //
///////////////////////////////////////

template <typename T, typename Comparator>
class RBTree {
 public:
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = Iterator<T>;
  using const_iterator = ConstIterator<T>;
  using size_type = std::size_t;
  using Node = RBTreeNode<T>;

  RBTree();
  RBTree(const RBTree& other);
  RBTree(RBTree&& other) noexcept;
  explicit RBTree(const Comparator& comp);

  ~RBTree();

  RBTree& operator=(const RBTree& other);
  RBTree& operator=(RBTree&& other) noexcept;

  void leftRotate(Node* node);
  void rightRotate(Node* node);
  void fixInsert(Node* node);
  void fixErase(Node* node);

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  bool empty() const;
  size_type size() const;
  size_type max_size() const;
  void printTree() const;
  void clear();

  std::pair<iterator, bool> insert(const value_type& value);
  std::pair<iterator, bool> insertNonUniq(const value_type& value);

  void erase(iterator pos);
  void swap(RBTree& other) noexcept;
  void merge(RBTree& other);
  void mergeNonUniq(RBTree& other);
  iterator find(const_reference key);
  iterator lower_bound(const_reference key);
  iterator upper_bound(const_reference key);
  const_iterator find(const_reference key) const;
  const_iterator lower_bound(const_reference key) const;
  const_iterator upper_bound(const_reference key) const;
  bool contains(const_reference key);

 private:
  void swapNode(Node* fNode, Node* sNode);
  Node* copyTree(Node* otherNode, Node* parentNode);
  void transplant(Node* u, Node* v);
  void deleteTree(Node* node);
  void printNodeValue(const_reference value) const;
  void printTree(Node* node, int depth = 0) const;
  Node* find_min(Node* root) const;
  Node* find_max(Node* root) const;

  Node* root_;
  Node* endNode_;
  size_type size_ = 0U;
  Comparator comp_;
};

// --- Implementations for RBTree ---

// --- Constructors/Destructor

template <typename T, typename Comparator>
RBTree<T, Comparator>::RBTree()
    : root_(nullptr), endNode_(new Node(nullptr)), size_(0), comp_() {}

template <typename T, typename Comparator>
RBTree<T, Comparator>::RBTree(const RBTree& other)
    : root_(nullptr),
      endNode_(new Node(nullptr)),
      size_(other.size_),
      comp_(other.comp_) {
  if (other.root_) {
    root_ = copyTree(other.root_, nullptr);
    Node* rightmost = root_;
    while (rightmost && rightmost->right_) {
      rightmost = rightmost->right_;
    }
    rightmost->right_ = endNode_;
    endNode_->parent_ = rightmost;
  }
}

template <typename T, typename Comparator>
RBTree<T, Comparator>::RBTree(RBTree&& other) noexcept
    : root_(other.root_),
      endNode_(other.endNode_),
      size_(other.size_),
      comp_(std::move(other.comp_)) {
  other.root_ = nullptr;

  other.endNode_ = new Node();
  other.endNode_->parent_ = nullptr;
  other.endNode_->left_ = nullptr;
  other.endNode_->right_ = nullptr;

  other.size_ = 0;
}

template <typename T, typename Comparator>
RBTree<T, Comparator>::RBTree(const Comparator& comp)
    : root_(nullptr), endNode_(new Node(nullptr)), size_(0), comp_(comp) {}

template <typename T, typename Comparator>
RBTree<T, Comparator>::~RBTree() {
  clear();
  delete endNode_;
}

template <typename T, typename Comparator>
RBTree<T, Comparator>& RBTree<T, Comparator>::operator=(const RBTree& other) {
  if (this != &other) {
    clear();
    root_ = copyTree(other.root_, nullptr);
    size_ = other.size_;
    if (!endNode_) {
      endNode_ = new Node();
    }
    endNode_->parent_ = find_max(root_);
    if (root_) {
      endNode_->parent_->right_ = endNode_;
    }
  }
  return *this;
}

template <typename T, typename Comparator>
RBTree<T, Comparator>& RBTree<T, Comparator>::operator=(
    RBTree&& other) noexcept {
  if (this != &other) {
    clear();
    root_ = other.root_;
    endNode_ = other.endNode_;
    size_ = other.size_;
    comp_ = std::move(other.comp_);
    other.root_ = nullptr;
    other.size_ = 0;
    other.endNode_ = new Node();
  }
  return *this;
}

/////

template <typename T, typename Comparator>
bool RBTree<T, Comparator>::empty() const {
  return size_ == 0;
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::size_type RBTree<T, Comparator>::size() const {
  return size_;
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::size_type RBTree<T, Comparator>::max_size()
    const {
  return std::numeric_limits<size_type>::max();
}

template <typename T, typename Comparator>
bool RBTree<T, Comparator>::contains(const_reference key) {
  return find(key) != end();
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::iterator RBTree<T, Comparator>::find(
    const_reference key) {
  Node* current = root_;
  while (current != endNode_ && current != nullptr) {
    if (comp_(key, current->value_)) {
      current = current->left_;
    } else if (comp_(current->value_, key)) {
      current = current->right_;
    } else {
      return iterator(current, endNode_);
    }
  }
  return end();
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::const_iterator RBTree<T, Comparator>::find(
    const_reference key) const {
  Node* current = root_;
  while (current != endNode_ && current != nullptr) {
    if (comp_(key, current->value_)) {
      current = current->left_;
    } else if (comp_(current->value_, key)) {
      current = current->right_;
    } else {
      return const_iterator(current, endNode_);
    }
  }
  return end();
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::iterator RBTree<T, Comparator>::lower_bound(
    const_reference key) {
  Node* current = root_;
  Node* result = nullptr;
  while (current != endNode_ && current != nullptr) {
    if (!comp_(current->value_, key)) {
      result = current;
      current = current->left_;
    } else {
      current = current->right_;
    }
  }
  return result ? iterator(result, endNode_) : end();
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::iterator RBTree<T, Comparator>::upper_bound(
    const_reference key) {
  Node* current = root_;
  Node* result = nullptr;
  while (current != endNode_ && current != nullptr) {
    if (comp_(key, current->value_)) {
      result = current;
      current = current->left_;
    } else {
      current = current->right_;
    }
  }
  return result ? iterator(result, endNode_) : end();
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::const_iterator
RBTree<T, Comparator>::lower_bound(const_reference key) const {
  Node* current = root_;
  Node* result = nullptr;
  while (current != endNode_ && current != nullptr) {
    if (!comp_(current->value_, key)) {
      result = current;
      current = current->left_;
    } else {
      current = current->right_;
    }
  }
  return result ? const_iterator(result, endNode_) : end();
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::const_iterator
RBTree<T, Comparator>::upper_bound(const_reference key) const {
  Node* current = root_;
  Node* result = nullptr;
  while (current != endNode_ && current != nullptr) {
    if (comp_(key, current->value_)) {
      result = current;
      current = current->left_;
    } else {
      current = current->right_;
    }
  }
  return result ? const_iterator(result, endNode_) : end();
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::iterator RBTree<T, Comparator>::begin() {
  return iterator(find_min(root_), endNode_);
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::iterator RBTree<T, Comparator>::end() {
  return iterator(endNode_, endNode_);
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::const_iterator RBTree<T, Comparator>::begin()
    const {
  return const_iterator(find_min(root_), endNode_);
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::const_iterator RBTree<T, Comparator>::end()
    const {
  return const_iterator(endNode_, endNode_);
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::swap(RBTree& other) noexcept {
  if (this == &other) return;
  std::swap(root_, other.root_);
  std::swap(endNode_, other.endNode_);
  std::swap(size_, other.size_);
  std::swap(comp_, other.comp_);
  if (root_) {
    endNode_->parent_ = find_max(root_);
    endNode_->parent_->right_ = endNode_;
  } else {
    endNode_->parent_ = nullptr;
  }
  if (other.root_) {
    other.endNode_->parent_ = find_max(other.root_);
    other.endNode_->parent_->right_ = other.endNode_;
  } else {
    other.endNode_->parent_ = nullptr;
  }
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::merge(RBTree& other) {
  if (nullptr == other.root_ || this == &other) return;
  bool merged = false;
  if (other.size_ > 0) {
    for (auto it = other.begin(); it != other.end(); ++it) {
      insert(*it);
      merged = true;
    }
  }
  if (merged) {
    other.clear();
  }
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::mergeNonUniq(RBTree& other) {
  if (nullptr == other.root_ || this == &other) return;
  bool merged = false;
  if (other.size_ > 0) {
    for (auto it = other.begin(); it != other.end(); ++it) {
      insertNonUniq(*it);
      merged = true;
    }
  }
  if (merged) {
    other.clear();
  }
}

template <typename T, typename Comparator>
std::pair<typename RBTree<T, Comparator>::iterator, bool>
RBTree<T, Comparator>::insertNonUniq(const value_type& value) {
  Node* newNode = new Node(value);
  Node* current = root_;
  Node* parent = nullptr;

  while (current != nullptr && current != endNode_) {
    parent = current;
    if (comp_(value, current->value_)) {
      current = current->left_;
    } else {
      current = current->right_;
    }
  }

  newNode->parent_ = parent;
  if (parent == nullptr) {
    root_ = newNode;
  } else if (comp_(value, parent->value_)) {
    parent->left_ = newNode;
  } else {
    parent->right_ = newNode;
  }
  fixInsert(newNode);
  if (root_ && endNode_) {
    Node* maxNode = find_max(root_);
    if (maxNode) {
      endNode_->parent_ = maxNode;
      endNode_->left_ = nullptr;
      maxNode->right_ = endNode_;
    }
  }
  ++size_;
  return std::make_pair(iterator(newNode), true);
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::erase(iterator pos) {
  if (!pos.isValid()) {
    throw std::runtime_error(
        "Trying to erase an invalid iterator: Iterator does not point to a "
        "valid node.");
  }

  Node* nodeToDelete = pos.getCurNode();
  if (!nodeToDelete || nodeToDelete == endNode_) return;

  Node* y = nodeToDelete;
  Node* x = nullptr;
  Color originalColor = y->color_;

  if (nodeToDelete->left_ == nullptr) {
    x = nodeToDelete->right_;
    transplant(nodeToDelete, nodeToDelete->right_);
  } else if (nodeToDelete->right_ == nullptr) {
    x = nodeToDelete->left_;
    transplant(nodeToDelete, nodeToDelete->left_);
  } else {
    y = find_min(nodeToDelete->right_);
    originalColor = y->color_;
    x = y->right_;

    if (y->parent_ == nodeToDelete) {
      if (x) x->parent_ = y;
    } else {
      transplant(y, y->right_);
      y->right_ = nodeToDelete->right_;
      if (y->right_) y->right_->parent_ = y;
    }

    transplant(nodeToDelete, y);
    y->left_ = nodeToDelete->left_;
    if (y->left_) y->left_->parent_ = y;
    y->color_ = nodeToDelete->color_;
  }

  if (originalColor == Color::BLACK) {
    fixErase(x);
  }

  delete nodeToDelete;
  --size_;

  if (root_) {
    Node* newMaxNode = find_max(root_);
    endNode_->parent_ = (newMaxNode == endNode_) ? nullptr : newMaxNode;
  } else {
    endNode_->parent_ = nullptr;
  }
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::transplant(Node* u, Node* v) {
  if (u->parent_ == nullptr) {
    root_ = v;
  } else if (u == u->parent_->left_) {
    u->parent_->left_ = v;
  } else {
    u->parent_->right_ = v;
  }

  if (v != nullptr) {
    v->parent_ = u->parent_;
  }
}
template <typename T, typename Comparator>
void RBTree<T, Comparator>::fixErase(Node* node) {
  while (node && node != root_ && node->color_ == Color::BLACK) {
    if (node == node->parent_->left_) {
      Node* sibling = node->parent_->right_;

      if (sibling->color_ == Color::RED) {
        sibling->color_ = Color::BLACK;
        node->parent_->color_ = Color::RED;
        leftRotate(node->parent_);
        sibling = node->parent_->right_;
        break;
      }

      if ((!sibling->left_ || sibling->left_ == endNode_ ||
           sibling->left_->color_ == Color::BLACK) &&
          (!sibling->right_ || sibling->right_ == endNode_ ||
           sibling->right_->color_ == Color::BLACK)) {
        sibling->color_ = Color::RED;
        node = node->parent_;
      } else {
        if (sibling->right_ == nullptr || sibling->right_ == endNode_ ||
            sibling->right_->color_ == Color::BLACK) {
          if (sibling->left_ && sibling->left_ != endNode_) {
            sibling->left_->color_ = Color::BLACK;
          }
          sibling->color_ = Color::RED;
          rightRotate(sibling);
          sibling = node->parent_->right_;
          break;
        }

        sibling->color_ = node->parent_->color_;
        node->parent_->color_ = Color::BLACK;
        if (sibling->right_ && sibling->right_ != endNode_) {
          sibling->right_->color_ = Color::BLACK;
        }
        leftRotate(node->parent_);
        node = root_;
      }
    } else {
      Node* sibling = node->parent_->left_;

      if (sibling->color_ == Color::RED) {
        sibling->color_ = Color::BLACK;
        node->parent_->color_ = Color::RED;
        rightRotate(node->parent_);
        sibling = node->parent_->left_;
        break;
      }

      if ((!sibling->right_ || sibling->right_ == endNode_ ||
           sibling->right_->color_ == Color::BLACK) &&
          (!sibling->left_ || sibling->left_ == endNode_ ||
           sibling->left_->color_ == Color::BLACK)) {
        sibling->color_ = Color::RED;
        node = node->parent_;
      } else {
        if (sibling->left_ == nullptr || sibling->left_ == endNode_ ||
            sibling->left_->color_ == Color::BLACK) {
          if (sibling->right_ && sibling->right_ != endNode_) {
            sibling->right_->color_ = Color::BLACK;
          }
          sibling->color_ = Color::RED;
          leftRotate(sibling);
          sibling = node->parent_->left_;
          break;
        }

        sibling->color_ = node->parent_->color_;
        node->parent_->color_ = Color::BLACK;
        if (sibling->left_ && sibling->left_ != endNode_) {
          sibling->left_->color_ = Color::BLACK;
        }
        rightRotate(node->parent_);
        node = root_;
      }
    }

    if (node != nullptr && node == endNode_) {
      Node* newMaxNode = find_max(root_);
      if (newMaxNode != nullptr) {
        endNode_->parent_ = newMaxNode;
      } else {
        endNode_->parent_ = nullptr;
      }
    }
  }
  if (node) node->color_ = Color::BLACK;
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::leftRotate(Node* node) {
  Node* rightChild = node->right_;
  if (!rightChild) {
    std::cerr << "Error: Attempted left rotation with a null right child."
              << std::endl;
    return;
  }

  node->right_ = rightChild->left_;
  if (rightChild->left_) {
    rightChild->left_->parent_ = node;
  }

  rightChild->parent_ = node->parent_;
  if (!node->parent_) {
    root_ = rightChild;
  } else if (node == node->parent_->left_) {
    node->parent_->left_ = rightChild;
  } else {
    node->parent_->right_ = rightChild;
  }

  rightChild->left_ = node;
  node->parent_ = rightChild;
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::rightRotate(Node* node) {
  Node* leftChild = node->left_;
  node->left_ = leftChild->right_;
  if (leftChild->right_) {
    leftChild->right_->parent_ = node;
  }
  leftChild->parent_ = node->parent_;
  if (!node->parent_) {
    root_ = leftChild;
  } else if (node == node->parent_->right_) {
    node->parent_->right_ = leftChild;
  } else {
    node->parent_->left_ = leftChild;
  }
  leftChild->right_ = node;
  node->parent_ = leftChild;
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::clear() {
  if (root_) {
    deleteTree(root_);
    root_ = nullptr;
  }
  endNode_->parent_ = nullptr;
  endNode_->left_ = nullptr;
  endNode_->right_ = nullptr;
  size_ = 0;
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::printTree() const {
  printTree(root_, 0);
}

// --- Private ---

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::Node* RBTree<T, Comparator>::find_min(
    Node* root) const {
  if (root == nullptr) return endNode_;
  while (root->left_ != nullptr) {
    root = root->left_;
  }
  return root;
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::Node* RBTree<T, Comparator>::find_max(
    Node* root) const {
  if (root == nullptr) return endNode_;
  while (root->right_ != endNode_ && root->right_ != nullptr) {
    root = root->right_;
  }
  return root;
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::Node* RBTree<T, Comparator>::copyTree(
    Node* otherNode, Node* parentNode) {
  if (!otherNode || otherNode == this->endNode_) return nullptr;

  Node* newNode = nullptr;
  try {
    newNode = new Node(otherNode->value_, parentNode, otherNode->color_);
    newNode->left_ = copyTree(otherNode->left_, newNode);
    newNode->right_ = copyTree(otherNode->right_, newNode);
  } catch (...) {
    deleteTree(newNode);
    throw;
  }

  return newNode;
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::swapNode(Node* fNode, Node* sNode) {
  if (!fNode || !sNode) return;
  if (fNode == endNode_ || sNode == endNode_) {
    if (fNode == endNode_) std::swap(fNode, sNode);
    if constexpr (is_pair_v<T>) {
      std::swap(fNode->value_.second, sNode->value_.second);
    } else {
      std::swap(fNode->value_, sNode->value_);
    }
    return;
  }
  if constexpr (is_pair_v<T>) {
    std::swap(fNode->value_.second, sNode->value_.second);
  } else {
    std::swap(fNode->value_, sNode->value_);
  }
  std::swap(fNode->color_, sNode->color_);
  if (fNode->parent_ == sNode) {
    std::swap(fNode, sNode);
    fNode->left_ = (fNode->left_ == sNode) ? sNode : fNode->left_;
    fNode->right_ = (fNode->right_ == sNode) ? sNode : fNode->right_;
  } else if (sNode->parent_ == fNode) {
    fNode->left_ = (sNode == fNode->left_) ? sNode : fNode->left_;
    fNode->right_ = (sNode == fNode->right_) ? sNode : fNode->right_;
  } else {
    std::swap(fNode->parent_, sNode->parent_);
    std::swap(fNode->left_, sNode->left_);
    std::swap(fNode->right_, sNode->right_);
  }
  if (fNode->parent_) {
    if (fNode->parent_->left_ == sNode)
      fNode->parent_->left_ = fNode;
    else if (fNode->parent_->right_ == sNode)
      fNode->parent_->right_ = fNode;
  }
  if (sNode->parent_) {
    if (sNode->parent_->left_ == fNode)
      sNode->parent_->left_ = sNode;
    else if (sNode->parent_->right_ == fNode)
      sNode->parent_->right_ = sNode;
  }
  if (fNode->left_) fNode->left_->parent_ = fNode;
  if (fNode->right_) fNode->right_->parent_ = fNode;
  if (sNode->left_) sNode->left_->parent_ = sNode;
  if (sNode->right_) sNode->right_->parent_ = sNode;
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::deleteTree(Node* node) {
  if (node && node != endNode_) {
    deleteTree(node->left_);
    deleteTree(node->right_);
    delete node;
  }
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::printNodeValue(const_reference value) const {
  if constexpr (is_pair_v<T>) {
    std::cout << "{" << value.first << ": " << value.second << "}";
  } else {
    std::cout << value;
  }
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::printTree(Node* node, int level) const {
  if (node == nullptr || node == endNode_) {
    if (node == endNode_) {
      std::cout << "endNode_->parent_: ";
      if (endNode_->parent_) {
        printNodeValue(endNode_->parent_->value_);
      } else {
        std::cout << "nullptr";
      }
      std::cout << std::endl;
    }
    return;
  }

  printTree(node->right_, level + 1);
  for (int i = 0; i < level; ++i) std::cout << "  ";
  printNodeValue(node->value_);
  std::cout << " (" << (node->color_ == Color::RED ? "R" : "B") << ")"
            << std::endl;
  printTree(node->left_, level + 1);
}

template <typename T, typename Comparator>
std::pair<typename RBTree<T, Comparator>::iterator, bool>
RBTree<T, Comparator>::insert(const value_type& value) {
  Node* newNode = new Node(value);
  Node* current = root_;
  Node* parent = nullptr;

  while (current != nullptr && current != endNode_) {
    parent = current;
    if (comp_(value, current->value_)) {
      current = current->left_;
    } else if (comp_(current->value_, value)) {
      current = current->right_;
    } else {
      delete newNode;
      return std::make_pair(iterator(current), false);
    }
  }

  newNode->parent_ = parent;
  if (parent == nullptr) {
    root_ = newNode;
  } else if (comp_(value, parent->value_)) {
    parent->left_ = newNode;
  } else {
    parent->right_ = newNode;
  }
  fixInsert(newNode);
  if (root_ && endNode_) {
    Node* maxNode = find_max(root_);
    if (maxNode && maxNode != endNode_) {
      endNode_->parent_ = maxNode;
      endNode_->left_ = nullptr;
      maxNode->right_ = endNode_;
    }
  }
  ++size_;
  return std::make_pair(iterator(newNode), true);
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::fixInsert(Node* node) {
  Node* parent = nullptr;
  Node* grandparent = nullptr;

  while (node != root_ && node->parent_->isRed()) {
    parent = node->parent_;
    grandparent = parent->parent_;

    if (parent == grandparent->left_) {
      Node* uncle = grandparent->right_;
      if (uncle != endNode_ && uncle != nullptr && uncle->isRed()) {
        parent->color_ = Color::BLACK;
        uncle->color_ = Color::BLACK;
        grandparent->color_ = Color::RED;
        node = grandparent;
      } else {
        if (node == parent->right_) {
          leftRotate(parent);
          node = parent;
          parent = node->parent_;
        }
        rightRotate(grandparent);
        std::swap(parent->color_, grandparent->color_);
        break;
      }
    } else {
      Node* uncle = grandparent->left_;
      if (uncle != endNode_ && uncle != nullptr && uncle->isRed()) {
        parent->color_ = Color::BLACK;
        uncle->color_ = Color::BLACK;
        grandparent->color_ = Color::RED;
        node = grandparent;
      } else {
        if (node == parent->left_) {
          rightRotate(parent);
          node = parent;
          parent = node->parent_;
        }
        leftRotate(grandparent);
        std::swap(parent->color_, grandparent->color_);
        break;
      }
    }
  }

  root_->color_ = Color::BLACK;
}

}  // namespace s21

#endif  // S21_RBTREE
