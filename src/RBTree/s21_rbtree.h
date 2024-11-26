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

  // Constructor for regular nodes
  RBTreeNode(const T& value, RBTreeNode* parent = nullptr,
             Color color = Color::RED)
      : value_(value),
        left_(nullptr),
        right_(nullptr),
        parent_(parent),
        color_(color) {}

  // Constructor for end node (doesn't initialize value_)
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

// template <typename T>
// Iterator<T>& Iterator<T>::operator--() {
//   if (current_node_ == nullptr) {
//     current_node_ = end_node_;
//     return *this;
//   }
//   if (current_node_ == end_node_) {
//     current_node_ = getMaxNode();
//     return *this;
//   }
//   if (current_node_->left_) {
//     current_node_ = current_node_->left_;
//     while (current_node_->right_) {
//       current_node_ = current_node_->right_;
//     }
//   } else {
//     Node* parent = current_node_->parent_;
//     while (parent && current_node_ == parent->left_) {
//       current_node_ = parent;
//       parent = parent->parent_;
//     }
//     current_node_ = parent ? parent : end_node_;
//   }

//   return *this;
// }

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

///

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

////

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
      endNode_(new Node(nullptr)),  // Create a fresh endNode_
      size_(other.size_),
      comp_(other.comp_) {
  if (other.root_) {
    root_ = copyTree(other.root_, nullptr);

    // Reconnect endNode_
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
    clear();  // Free existing resources
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
      insertNonUniq(*it);  // method for non-unique
      merged = true;
    }
  }
  if (merged) {
    other.clear();  // Clear other tree only if merge happened!!
  }
}

// template <typename T, typename Comparator>
// std::pair<typename RBTree<T, Comparator>::iterator, bool>
// RBTree<T, Comparator>::insert(const value_type& value) {
//   // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//   std::cout << "Inserting value: " << value << std::endl;
//   // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//   Node* newNode = new Node(value);  // Create new node with value

//   if (root_ == nullptr) {
//     // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//     std::cout << "Tree is empty. Setting root and endNode_." << std::endl;
//     // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//     root_ = newNode;              // If tree is empty, new node becomes
//     root root_->parent_ = endNode_;    // Connect root to sentinel
//     endNode_->parent_ = newNode;  // Set endNode to point to new root
//   } else {
//     Node* node = root_;
//     while (node != endNode_) {
//       // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//       std::cout << "Traversing tree: current node = " << node->value_
//                 << std::endl;
//       // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//       if (comp_(value, node->value_)) {
//         if (node->left_ == nullptr || node->left_ == endNode_) {
//           node->left_ = newNode;
//           newNode->parent_ = node;
//           break;
//         }
//         node = node->left_;
//       } else if (comp_(node->value_, value)) {
//         if (node->right_ == nullptr || node->right_ == endNode_) {
//           node->right_ = newNode;
//           newNode->parent_ = node;
//           break;
//         }
//         node = node->right_;
//       } else {
//         // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//         std::cout << "Value already exists. Skipping insertion." <<
//         std::endl;
//         // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//         delete newNode;  // Value already exists, no insertion
//         return std::make_pair(iterator(node, endNode_), false);
//       }
//     }
//   }

//   // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//   std::cout << "Node inserted. Launching fixInsert." << std::endl;
//   // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//   fixInsert(newNode);

//   // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//   std::cout << "Insertion complete. endNode_->parent_: ";
//   if (endNode_->parent_) {
//     std::cout << endNode_->parent_->value_;
//   } else {
//     std::cout << "nullptr";
//   }
//   std::cout << std::endl;
//   std::cout << "Printing tree: " << std::endl;
//   printTree(root_);
//   // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//   ++size_;
//   return std::make_pair(iterator(newNode, endNode_), true);
// }

// template <typename T, typename Comparator>
// std::pair<typename RBTree<T, Comparator>::iterator, bool>
// RBTree<T, Comparator>::insertNonUniq(const value_type& value) {
//   Node* newNode = new Node(value);
//   if (root_ == nullptr) {
//     root_ = newNode;
//     root_->parent_ = endNode_;    // Connect root to sentinel
//     endNode_->parent_ = newNode;  // Set endNode to point to new root
//   } else {
//     Node* node = root_;
//     while (node != endNode_) {
//       if (comp_(value, node->value_)) {
//         if (node->left_ == nullptr || node->left_ == endNode_) {
//           node->left_ = newNode;
//           newNode->parent_ = node;
//           break;
//         }
//         node = node->left_;
//       } else {
//         if (node->right_ == nullptr || node->right_ == endNode_) {
//           node->right_ = newNode;
//           newNode->parent_ = node;
//           break;
//         }
//         node = node->right_;
//       }
//     }
//     if (comp_(endNode_->parent_->value_, newNode->value_)) {
//       endNode_->parent_ = find_max(root_);
//     }
//   }

//   fixInsert(newNode);  // Rebalance the tree
//   ++size_;
//   return std::make_pair(iterator(newNode, endNode_), true);
// }

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

// template <typename T, typename Comparator>
// void RBTree<T, Comparator>::fixInsert(Node* node) {
//   // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//   std::cout << "Fixing insertion for node: " << node->value_ << std::endl;
//   // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//   while (node != root_ && node->parent_ != endNode_ &&
//   node->parent_->isRed()) {
//     // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//     std::cout << "At node: " << node->value_
//               << ", parent: " << node->parent_->value_ << ", grandparent:
//               ";
//     if (node->parent_->parent_) {
//       std::cout << node->parent_->parent_->value_;
//     } else {
//       std::cout << "nullptr";
//     }
//     std::cout << std::endl;
//     // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//     if (node->parent_ == node->parent_->parent_->left_) {
//       // If parent is left child of grandparent
//       Node* uncle = node->parent_->parent_->right_;
//       if (uncle && uncle != endNode_ && uncle->isRed()) {
//         // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//         std::cout << "Case 1: Recoloring and moving up." << std::endl;
//         // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//         // Case 1: Uncle is red, recolor and move up
//         node->parent_->color_ = Color::BLACK;
//         uncle->color_ = Color::BLACK;
//         node->parent_->parent_->color_ = Color::RED;
//         node = node->parent_->parent_;
//       } else {
//         // Case 2: Uncle is black
//         if (node == node->parent_->right_) {
//           // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//           std::cout << "Case 2a: Left rotation needed." << std::endl;

//           // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//           // Case 2a: Node is right child, need to rotate left
//           node = node->parent_;
//           leftRotate(node);
//         }

//         // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//         std::cout << "Case 2b: Right rotation needed." << std::endl;
//         // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//         // Case 2b: Node is left child, need to rotate right
//         node->parent_->color_ = Color::BLACK;
//         node->parent_->parent_->color_ = Color::RED;
//         rightRotate(node->parent_->parent_);
//       }
//     } else {
//       // mirror of the above case
//       Node* uncle = node->parent_->parent_->left_;
//       if (uncle && uncle != endNode_ && uncle->isRed()) {
//         // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//         std::cout << "Case 1 (mirror): Recoloring and moving up." <<
//         std::endl;
//         // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//         // Case 1:
//         node->parent_->color_ = Color::BLACK;
//         uncle->color_ = Color::BLACK;
//         node->parent_->parent_->color_ = Color::RED;
//         node = node->parent_->parent_;
//       } else {
//         // Case 2:
//         if (node == node->parent_->left_) {
//           // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//           std::cout << "Case 2a (mirror): Right rotation needed." <<
//           std::endl;
//           // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//           // Case 2a:
//           node = node->parent_;
//           rightRotate(node);
//         }

//         // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//         std::cout << "Case 2b (mirror): Left rotation needed." <<
//         std::endl;
//         // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//         // Case 2b:
//         node->parent_->color_ = Color::BLACK;
//         node->parent_->parent_->color_ = Color::RED;
//         leftRotate(node->parent_->parent_);
//       }
//     }
//   }
//   root_->color_ = Color::BLACK;  // Root is always black!

//   // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//   // std::cout << "FixInsert complete. Updating endNode_->parent_." <<
//   // std::endl;
//   // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//   // if (endNode_->parent_ != nullptr) {
//   //   endNode_->parent_ = find_max(root_);
//   // }
//   // if (endNode_ && root_) {
//   //   endNode_->parent_ = find_max(root_);
//   // }
// }
//
//

//
//
//
//
//

template <typename T, typename Comparator>
void RBTree<T, Comparator>::erase(iterator pos) {
  if (!pos.isValid()) {
    throw std::runtime_error(
        "Trying to erase an invalid iterator: Iterator does not point to a "
        "valid node.");
  }

  Node* nodeToDelete = pos.getCurNode();
  if (!nodeToDelete || nodeToDelete == endNode_) return;

  Node* y = nodeToDelete;  // Node to be actually removed
  Node* x = nullptr;       // Child of y, possibly null
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

  // Update endNode_
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

// template <typename T, typename Comparator>
// void RBTree<T, Comparator>::erase(iterator pos) {
//   if (pos == end()) {
//     return;  // Cannot erase the end iterator
//   }

//   Node* nodeToErase = pos.getCurNode();
//   Node* x = nullptr;
//   Node* y = nodeToErase;  // y will be the node to be unlinked from the tree
//   Color yOriginalColor = y->color_;

//   if (nodeToErase->left_ == nullptr) {
//     x = nodeToErase->right_;
//     transplant(nodeToErase, nodeToErase->right_);
//   } else if (nodeToErase->right_ == nullptr) {
//     x = nodeToErase->left_;
//     transplant(nodeToErase, nodeToErase->left_);
//   } else {
//     y = find_min(nodeToErase->right_);
//     yOriginalColor = y->color_;
//     x = y->right_;
//     if (y->parent_ == nodeToErase) {
//       x->parent_ = y;
//     } else {
//       transplant(y, y->right_);
//       y->right_ = nodeToErase->right_;
//       y->right_->parent_ = y;
//     }
//     transplant(nodeToErase, y);
//     y->left_ = nodeToErase->left_;
//     y->left_->parent_ = y;
//     y->color_ = nodeToErase->color_;
//   }

//   if (yOriginalColor == Color::BLACK) {
//     fixErase(x);
//   }

//   delete nodeToErase;
//   --size_;
// }

// template <typename T, typename Comparator>
// void RBTree<T, Comparator>::transplant(Node* u, Node* v) {
//   if (u->parent_ == nullptr) {
//     root_ = v;
//   } else if (u == u->parent_->left_) {
//     u->parent_->left_ = v;
//   } else {
//     u->parent_->right_ = v;
//   }
//   if (v != nullptr) {
//     v->parent_ = u->parent_;
//   }
// }

// template <typename T, typename Comparator>
// void RBTree<T, Comparator>::fixErase(Node* x) {
//   while (x != root_ && (x == nullptr || x->isBlack())) {
//     if (x == x->parent_->left_) {
//       Node* w = x->parent_->right_;
//       if (w->isRed()) {
//         w->color_ = Color::BLACK;
//         x->parent_->color_ = Color::RED;
//         leftRotate(x->parent_);
//         w = x->parent_->right_;
//       }
//       if ((w->left_ == nullptr || w->left_->isBlack()) &&
//           (w->right_ == nullptr || w->right_->isBlack())) {
//         w->color_ = Color::RED;
//         x = x->parent_;
//       } else {
//         if (w->right_ == nullptr || w->right_->isBlack()) {
//           if (w->left_ != nullptr) {
//             w->left_->color_ = Color::BLACK;
//           }
//           w->color_ = Color::RED;
//           rightRotate(w);
//           w = x->parent_->right_;
//         }
//         w->color_ = x->parent_->color_;
//         x->parent_->color_ = Color::BLACK;
//         if (w->right_ != nullptr) {
//           w->right_->color_ = Color::BLACK;
//         }
//         leftRotate(x->parent_);
//         x = root_;
//       }
//     } else {
//       Node* w = x->parent_->left_;
//       if (w->isRed()) {
//         w->color_ = Color::BLACK;
//         x->parent_->color_ = Color::RED;
//         rightRotate(x->parent_);
//         w = x->parent_->left_;
//       }
//       if ((w->right_ == nullptr || w->right_->isBlack()) &&
//           (w->left_ == nullptr || w->left_->isBlack())) {
//         w->color_ = Color::RED;
//         x = x->parent_;
//       } else {
//         if (w->left_ == nullptr || w->left_->isBlack()) {
//           if (w->right_ != nullptr) {
//             w->right_->color_ = Color::BLACK;
//           }
//           w->color_ = Color::RED;
//           leftRotate(w);
//           w = x->parent_->left_;
//         }
//         w->color_ = x->parent_->color_;
//         x->parent_->color_ = Color::BLACK;
//         if (w->left_ != nullptr) {
//           w->left_->color_ = Color::BLACK;
//         }
//         rightRotate(x->parent_);
//         x = root_;
//       }
//     }
//   }
//   if (x != nullptr) {
//     x->color_ = Color::BLACK;
//   }
// }
template <typename T, typename Comparator>
void RBTree<T, Comparator>::fixErase(Node* node) {
  while (node && node != root_ && node->color_ == Color::BLACK) {
    if (node == node->parent_->left_) {
      Node* sibling = node->parent_->right_;

      if (sibling->color_ == Color::RED) {
        // Case 1: Sibling is red, rotate
        sibling->color_ = Color::BLACK;
        node->parent_->color_ = Color::RED;
        leftRotate(node->parent_);
        sibling = node->parent_->right_;
        break;  // DELETE PROBABLY
      }

      // Case 2: Both sibling's children are black or null
      if ((!sibling->left_ || sibling->left_ == endNode_ ||
           sibling->left_->color_ == Color::BLACK) &&
          (!sibling->right_ || sibling->right_ == endNode_ ||
           sibling->right_->color_ == Color::BLACK)) {
        sibling->color_ = Color::RED;
        node = node->parent_;
      } else {
        if (sibling->right_ == nullptr || sibling->right_ == endNode_ ||
            sibling->right_->color_ == Color::BLACK) {
          // Case 3: Sibling's left child is red, right is black
          if (sibling->left_ && sibling->left_ != endNode_) {
            sibling->left_->color_ = Color::BLACK;
          }
          sibling->color_ = Color::RED;
          rightRotate(sibling);
          sibling = node->parent_->right_;
          break;  // DELETE PROBABLY
        }

        // Case 4: Sibling's right child is red
        sibling->color_ = node->parent_->color_;
        node->parent_->color_ = Color::BLACK;
        if (sibling->right_ && sibling->right_ != endNode_) {
          sibling->right_->color_ = Color::BLACK;
        }
        leftRotate(node->parent_);
        node = root_;
      }
    } else {
      // Symmetric to the above
      Node* sibling = node->parent_->left_;

      if (sibling->color_ == Color::RED) {
        // Case 1: Sibling is red, rotate
        sibling->color_ = Color::BLACK;
        node->parent_->color_ = Color::RED;
        rightRotate(node->parent_);
        sibling = node->parent_->left_;
        break;  // DELETE PROBABLY
      }

      // Case 2: Both sibling's children are black or null
      if ((!sibling->right_ || sibling->right_ == endNode_ ||
           sibling->right_->color_ == Color::BLACK) &&
          (!sibling->left_ || sibling->left_ == endNode_ ||
           sibling->left_->color_ == Color::BLACK)) {
        sibling->color_ = Color::RED;
        node = node->parent_;
      } else {
        if (sibling->left_ == nullptr || sibling->left_ == endNode_ ||
            sibling->left_->color_ == Color::BLACK) {
          // Case 3: Sibling's right child is red, left is black
          if (sibling->right_ && sibling->right_ != endNode_) {
            sibling->right_->color_ = Color::BLACK;
          }
          sibling->color_ = Color::RED;
          leftRotate(sibling);
          sibling = node->parent_->left_;
          break;  // DELETE PROBABLY
        }

        // Case 4: Sibling's left child is red
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
      // Find the new maximum node after deletion
      Node* newMaxNode = find_max(root_);
      if (newMaxNode != nullptr) {
        // Adjust endNode's parent to point to the new maximum node
        endNode_->parent_ = newMaxNode;
      } else {
        // If the tree is empty, reset endNode to nullptr
        endNode_->parent_ = nullptr;
      }
    }
  }
  if (node) node->color_ = Color::BLACK;
}

// template <typename T, typename Comparator>
// void RBTree<T, Comparator>::fixErase(Node* node) {
//   // // // DELETE vvvvvvvv
//   std::cout << "Node: " << node << ", Parent: " << node->parent_ <<
//   std::endl;
//   // // // DELETE ^^^^^^
//   while (node != root_ && node->color_ == Color::BLACK) {
//     if (node == node->parent_->left_) {
//       Node* sibling = node->parent_->right_;
//       if (!sibling || sibling == endNode_) {
//         // Handle null or endNode case
//         node = root_;
//         continue;
//       }

//       if (sibling->color_ == Color::RED) {
//         // Case 1: Sibling is red, rotate
//         sibling->color_ = Color::BLACK;
//         node->parent_->color_ = Color::RED;
//         leftRotate(node->parent_);
//         sibling = node->parent_->right_;
//       }

//       if (sibling->left_ == nullptr || sibling->left_ == endNode_ ||
//           sibling->left_->color_ == Color::BLACK) {
//         if (sibling->right_ == nullptr || sibling->right_ == endNode_ ||
//             sibling->right_->color_ == Color::BLACK) {
//           // Case 2: Both of sibling's children are black or null
//           sibling->color_ = Color::RED;
//           node = node->parent_;
//         } else {
//           if (sibling->right_ == nullptr || sibling->right_ == endNode_ ||
//               sibling->right_->color_ == Color::BLACK) {
//             // Case 3: Sibling's left child is red, right is black
//             sibling->left_->color_ = Color::BLACK;
//             sibling->color_ = Color::RED;
//             rightRotate(sibling);
//             sibling = node->parent_->right_;
//           }
//           // Case 4: Sibling's right child is red
//           sibling->color_ = node->parent_->color_;
//           node->parent_->color_ = Color::BLACK;
//           if (sibling->right_ != endNode_ && sibling->right_ != nullptr) {
//             sibling->right_->color_ = Color::BLACK;
//           }
//           leftRotate(node->parent_);
//           node = root_;
//         }
//       } else {
//         // Case 5: Sibling's left child is red
//         sibling->color_ = node->parent_->color_;
//         node->parent_->color_ = Color::BLACK;
//         if (sibling->left_ != nullptr && sibling->left_ != endNode_) {
//           sibling->left_->color_ = Color::BLACK;
//         }
//         leftRotate(node->parent_);
//         node = root_;
//       }
//     } else {
//       // Symmetric to the above
//       Node* sibling = node->parent_->left_;

//       if (sibling->color_ == Color::RED) {
//         // Case 1:
//         sibling->color_ = Color::BLACK;
//         node->parent_->color_ = Color::RED;
//         rightRotate(node->parent_);
//         sibling = node->parent_->left_;
//       }

//       if (sibling->right_ == nullptr || sibling->right_ == endNode_ ||
//           sibling->right_->color_ == Color::BLACK) {
//         if (sibling->left_ == nullptr || sibling->left_ == endNode_ ||
//             sibling->left_->color_ == Color::BLACK) {
//           // Case 2:
//           sibling->color_ = Color::RED;
//           node = node->parent_;
//         } else {
//           if (sibling->left_->color_ == Color::BLACK) {
//             // Case 3:
//             sibling->right_->color_ = Color::BLACK;
//             sibling->color_ = Color::RED;
//             leftRotate(sibling);
//             sibling = node->parent_->left_;
//           }

//           // Case 4:
//           sibling->color_ = node->parent_->color_;
//           node->parent_->color_ = Color::BLACK;
//           if (sibling->left_ != nullptr && sibling->left_ != endNode_) {
//             sibling->left_->color_ = Color::BLACK;
//           }
//           rightRotate(node->parent_);
//           node = root_;
//         }
//       } else {
//         // Case 5:
//         sibling->color_ = node->parent_->color_;
//         node->parent_->color_ = Color::BLACK;
//         if (sibling->right_ != nullptr && sibling->right_ != endNode_) {
//           sibling->right_->color_ = Color::BLACK;
//         }
//         rightRotate(node->parent_);
//         node = root_;
//       }
//     }

//     if (node != nullptr && node == endNode_) {
//       // Find the new maximum node after deletion
//       Node* newMaxNode = find_max(root_);
//       if (newMaxNode != nullptr) {
//         // Adjust endNode's parent to point to the new maximum node
//         endNode_->parent_ = newMaxNode;
//       } else {
//         // If the tree is empty, reset endNode to nullptr
//         endNode_->parent_ = nullptr;
//       }
//     }
//   }

//   node->color_ = Color::BLACK;
// }

// template <typename T, typename Comparator>
// void RBTree<T, Comparator>::leftRotate(Node* node) {
//   // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//   std::cout << "Left rotating at node: " << node->value_ << std::endl;
//   std::cout << "Before rotation: \n";
//   printTree(root_);
//   // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
//   Node* rightChild = node->right_;
//   node->right_ = rightChild->left_;
//   if (rightChild->left_) {
//     rightChild->left_->parent_ = node;
//   }
//   rightChild->parent_ = node->parent_;
//   if (!node->parent_) {
//     root_ = rightChild;
//   } else if (node == node->parent_->left_) {
//     node->parent_->left_ = rightChild;
//   } else {
//     node->parent_->right_ = rightChild;
//   }
//   rightChild->left_ = node;
//   node->parent_ = rightChild;

//   // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
//   std::cout << "Left rotation complete. New root: " << root_->value_
//             << std::endl;
//   std::cout << "After rotation: \n";
//   printTree(root_);
//   // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
// }

template <typename T, typename Comparator>
void RBTree<T, Comparator>::leftRotate(Node* node) {
  //   std::cout << "Left rotating at node: " << node->value_ << std::endl;
  //   std::cout << "Before rotation: \n";
  //   printTree(root_);

  Node* rightChild = node->right_;
  if (!rightChild) {
    std::cerr << "Error: Attempted left rotation with a null right child."
              << std::endl;
    return;
  }

  // Step 1: Reassign node's right child to rightChild's left child
  node->right_ = rightChild->left_;
  if (rightChild->left_) {
    rightChild->left_->parent_ = node;
  }

  // Step 2: Reassign rightChild's parent to node's parent
  rightChild->parent_ = node->parent_;
  if (!node->parent_) {
    // If node is the root, update root
    root_ = rightChild;
  } else if (node == node->parent_->left_) {
    // If node is a left child
    node->parent_->left_ = rightChild;
  } else {
    // If node is a right child
    node->parent_->right_ = rightChild;
  }

  // Step 3: Make node the left child of rightChild
  rightChild->left_ = node;
  node->parent_ = rightChild;

  //   std::cout << "Left rotation complete. New root: " << root_->value_
  //             << std::endl;
  //   std::cout << "After rotation: \n";
  //   printTree(root_);
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::rightRotate(Node* node) {
  //   // // // DELETE DELETE DELETE   ↓↓↓↓↓↓
  //   std::cout << "Right rotating at node: " << node->value_ << std::endl;
  //   // // // DELETE DELETE DELETE   ↑↑↑↑↑↑
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
  //   std::cout << "Right rotation complete. New root: " << root_->value_
  //             << std::endl;
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::clear() {
  if (root_) {
    deleteTree(root_);
    root_ = nullptr;
  }
  // Reset end node to initial state
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
  // Traverse to leftmost node.
  while (root->left_ != nullptr) {
    root = root->left_;
  }
  return root;
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::Node* RBTree<T, Comparator>::find_max(
    Node* root) const {
  if (root == nullptr) return endNode_;
  // Traverse to rightmost node.
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
  if (node && node != endNode_) {  // Skip endNode_
    deleteTree(node->left_);
    deleteTree(node->right_);
    delete node;
  }
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::printNodeValue(const_reference value) const {
  if constexpr (is_pair_v<T>) {
    // For std::pair, print both key and value
    std::cout << "{" << value.first << ": " << value.second << "}";
  } else {
    // For non-pair types
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

      //   // Print value for the node before endNode_
      //   auto it = endNode_;
      //   --it;  // Assuming valid iterator implementation
      //   std::cout << "--endNode value: ";
      //   printNodeValue(it->value_);
      //   std::cout << std::endl;
    }
    return;
  }

  printTree(node->right_, level + 1);
  // Indent based on tree level
  for (int i = 0; i < level; ++i) std::cout << "  ";

  // Print current node's value
  printNodeValue(node->value_);
  std::cout << " (" << (node->color_ == Color::RED ? "R" : "B") << ")"
            << std::endl;

  // Recursively print left and right children
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
        // Case 1: Recolor
        parent->color_ = Color::BLACK;
        uncle->color_ = Color::BLACK;
        grandparent->color_ = Color::RED;
        node = grandparent;
      } else {
        if (node == parent->right_) {
          // Case 2: Left rotate
          leftRotate(parent);
          node = parent;
          parent = node->parent_;
        }
        // Case 3: Right rotate
        rightRotate(grandparent);
        std::swap(parent->color_, grandparent->color_);
        break;
      }
    } else {
      Node* uncle = grandparent->left_;
      if (uncle != endNode_ && uncle != nullptr && uncle->isRed()) {
        // Case 1: Recolor
        parent->color_ = Color::BLACK;
        uncle->color_ = Color::BLACK;
        grandparent->color_ = Color::RED;
        node = grandparent;
      } else {
        if (node == parent->left_) {
          // Case 2: Right rotate
          rightRotate(parent);
          node = parent;
          parent = node->parent_;
        }
        // Case 3: Left rotate
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
