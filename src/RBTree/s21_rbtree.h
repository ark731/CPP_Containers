#ifndef S21_RBTREE
#define S21_RBTREE

#include <memory>
#include <utility>

namespace s21 {

template <typename T>
class ConstIterator;

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

////////////////////////////////////
// --------- ITERATOR ----------- //
////////////////////////////////////

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

  Node* getCurNode() const;
  bool isValid() const;
  ConstIterator<T> toConstIterator() const {
    return ConstIterator<T>(current_node_, end_node_);
  }

  Iterator& operator=(const Iterator& other) = default;
  Iterator& operator=(Iterator&& other) noexcept = default;
  reference operator*();
  value_type* operator->();
  bool operator==(const Iterator& other) const;
  bool operator!=(const Iterator& other) const;
  Iterator& operator++();
  Iterator& operator--();
  Iterator operator++(int);
  Iterator operator--(int);

 protected:
  Node* current_node_ = nullptr;
  Node* end_node_ = nullptr;
};

// --- Implementations for Iterator ---

template <typename T>
typename Iterator<T>::Node* Iterator<T>::getCurNode() const {
  return current_node_;
}

template <typename T>
bool Iterator<T>::isValid() const {
  return current_node_ != nullptr;
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
  if (current_node_ == end_node_) {
    current_node_ = find_max(root_);
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
  using Node = RBTreeNode<value_type>;

  ConstIterator() = default;
  ConstIterator(Node* cur, Node* end) : Iterator<T>(cur, end) {}
  ConstIterator(Node* cur) : Iterator<T>(cur, nullptr) {}
  ConstIterator(const Iterator<T>& other) : Iterator<T>(other) {}
  ConstIterator(const ConstIterator& other) = default;
  ConstIterator(ConstIterator&& other) noexcept = default;
  ConstIterator& operator=(const ConstIterator& other) = default;
  ConstIterator& operator=(ConstIterator&& other) noexcept = default;
  const value_type* operator->() const;
  const_reference operator*() const;
  ConstIterator next() const;
  ConstIterator prev() const;
};

// --- Implementations for Const Iterator ---

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

  RBTree();
  RBTree(const RBTree& other);
  RBTree(RBTree&& other) noexcept;
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
  void swap(RBTree& other);
  void merge(RBTree& other);
  void mergeNonUniq(RBTree& other);
  iterator find(const_reference key);
  iterator lower_bound(const_reference key);
  iterator upper_bound(const_reference key);
  bool contains(const_reference key);

 private:
  value_type& getCurrentValue() const;
  void swapNode(Node* fNode, Node* sNode);
  Node* copyTree(Node* otherNode, Node* parentNode);
  void deleteTree(Node* node);
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
    : root_(nullptr), endNode_(new Node()), size_(0), comp_() {}

template <typename T, typename Comparator>
RBTree<T, Comparator>::RBTree(const RBTree& other)
    : root_(nullptr),
      endNode_(new Node()),
      size_(other.size_),
      comp_(other.comp_) {
  if (other.root_) {
    root_ = copyTree(other.root_, nullptr);
  }
}

template <typename T, typename Comparator>
RBTree<T, Comparator>::RBTree(RBTree&& other) noexcept
    : root_(other.root_),
      endNode_(other.endNode_),
      size_(other.size_),
      comp_(std::move(other.comp_)) {
  other.root_ = nullptr;
  other.endNode_ = nullptr;
  other.size_ = 0;
}

template <typename T, typename Comparator>
RBTree<T, Comparator>::~RBTree() {
  clear();
  delete endNode_;
}

// --- Copy/Move constructors

template <typename T, typename Comparator>
RBTree<T, Comparator>& RBTree<T, Comparator>::operator=(const RBTree& other) {
  if (this != &other) {
    clear();  // Deep copy
    root_ = copyTree(other.root_, nullptr);
    size_ = other.size_;
    endNode_ = new Node();
    endNode_->parent_ = find_max(root_);
    root_->parent_ = endNode_;
  }
  return *this;
}

template <typename T, typename Comparator>
RBTree<T, Comparator>& RBTree<T, Comparator>::operator=(
    RBTree&& other) noexcept {
  if (this != &other) {
    clear();  // Transfer ownership
    root_ = other.root_;
    endNode_ = other.endNode_;
    size_ = other.size_;
    other.root_ = nullptr;
    other.endNode_ = nullptr;
    other.size_ = 0;
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
  while (current != nullptr) {
    if (comp_(key, current->value_)) {
      current = current->left_;
    } else if (comp_(current->value_, key)) {
      current = current->right_;
    } else {
      return iterator(current, endNode_);  // Key found
    }
  }
  return end();  // Key not found
}

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
  while (root->right_ != nullptr) {
    root = root->right_;
  }
  return root;
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::iterator RBTree<T, Comparator>::lower_bound(
    const_reference key) {
  Node* current = root_;
  Node* result = nullptr;
  while (current != nullptr) {
    if (!comp_(current->value_, key)) {  // `current->value_ >= key`
      result = current;
      current = current->left_;  // Move left to find smaller
    } else {
      current = current->right_;  // Move right to find larger
    }
  }
  return result ? iterator(result, endNode_) : end();
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::iterator RBTree<T, Comparator>::upper_bound(
    const_reference key) {
  Node* current = root_;
  Node* result = nullptr;
  while (current != nullptr) {
    if (comp_(key, current->value_)) {  // `current->value_ > key`
      result = current;
      current = current->left_;
    } else {
      current = current->right_;
    }
  }
  return result ? iterator(result, endNode_) : end();
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
void RBTree<T, Comparator>::swap(RBTree& other) {
  std::swap(root_, other.root_);
  std::swap(endNode_, other.endNode_);
  std::swap(size_, other.size_);
  std::swap(comp_, other.comp_);
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::merge(RBTree& other) {
  if (nullptr == other.root_ || this == &other) return;
  bool merged = false;
  if (other.size_ > 0) {
    for (auto it = other.begin(); it != other.end(); ++it) {
      insert(*it);    // method for unique
      merged = true;  // Flag that merge happened
    }
  }
  if (merged) {
    other.clear();  // Clear other tree only if merge happened!!
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

template <typename T, typename Comparator>
std::pair<typename RBTree<T, Comparator>::iterator, bool>
RBTree<T, Comparator>::insert(const value_type& value) {
  Node* newNode = new Node(value);  // Create new node with value

  if (root_ == nullptr) {
    root_ = newNode;  // If tree is empty, new node becomes root
  } else {
    Node* node = root_;
    while (true) {
      if (comp_(value, node->value_)) {
        if (node->left_ == nullptr) {
          node->left_ = newNode;
          newNode->parent_ = node;
          break;
        } else {
          node = node->left_;
        }
      } else if (comp_(node->value_, value)) {
        if (node->right_ == nullptr) {
          node->right_ = newNode;
          newNode->parent_ = node;
          break;
        } else {
          node = node->right_;
        }
      } else {
        delete newNode;  // Value already exists, no insertion
        return std::make_pair(iterator(node, endNode_), false);
      }
    }
  }

  fixInsert(newNode);

  ++size_;
  root_->parent_ = endNode_;            // Set parent to sentinel
  endNode_->parent_ = find_max(root_);  // Set parent of endNode to  max element
  return std::make_pair(iterator(newNode, endNode_), true);
}

template <typename T, typename Comparator>
std::pair<typename RBTree<T, Comparator>::iterator, bool>
RBTree<T, Comparator>::insertNonUniq(const value_type& value) {
  Node* newNode = new Node(value);  // Create new node with value
  if (root_ == nullptr) {
    // If tree is empty, make newNode root
    root_ = newNode;
  } else {
    Node* node = root_;
    while (true) {
      if (comp_(value, node->value_)) {
        if (node->left_ == nullptr) {
          node->left_ = newNode;
          newNode->parent_ = node;
          break;
        } else {
          node = node->left_;
        }
      } else {
        if (node->right_ == nullptr) {
          node->right_ = newNode;
          newNode->parent_ = node;
          break;
        } else {
          node = node->right_;
        }
      }
    }
  }

  fixInsert(newNode);

  ++size_;
  root_->parent_ = endNode_;
  return std::make_pair(iterator(newNode, endNode_), true);
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::fixInsert(Node* node) {
  while (node != root_ && node->parent_->isRed()) {
    if (node->parent_ == node->parent_->parent_->left_) {
      // If parent is left child of grandparent
      Node* uncle = node->parent_->parent_->right_;
      if (uncle && uncle->isRed()) {
        // Case 1: Uncle is red, recolor and move up
        node->parent_->color_ = Color::BLACK;
        uncle->color_ = Color::BLACK;
        node->parent_->parent_->color_ = Color::RED;
        node = node->parent_->parent_;
      } else {
        // Case 2: Uncle is black
        if (node == node->parent_->right_) {
          // Case 2a: Node is right child, need to rotate left
          node = node->parent_;
          leftRotate(node);
        }
        // Case 2b: Node is left child, need to rotate right
        node->parent_->color_ = Color::BLACK;
        node->parent_->parent_->color_ = Color::RED;
        rightRotate(node->parent_->parent_);
      }
    } else {
      // mirror of the above case
      Node* uncle = node->parent_->parent_->left_;
      if (uncle && uncle->isRed()) {
        // Case 1:
        node->parent_->color_ = Color::BLACK;
        uncle->color_ = Color::BLACK;
        node->parent_->parent_->color_ = Color::RED;
        node = node->parent_->parent_;
      } else {
        // Case 2:
        if (node == node->parent_->left_) {
          // Case 2a:
          node = node->parent_;
          rightRotate(node);
        }
        // Case 2b:
        node->parent_->color_ = Color::BLACK;
        node->parent_->parent_->color_ = Color::RED;
        leftRotate(node->parent_->parent_);
      }
    }
  }
  root_->color_ = Color::BLACK;  // Root is always black!
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::erase(iterator pos) {
  if (!pos.isValid()) {
    throw std::runtime_error("Trying to erase an invalid iterator.");
  }

  Node* nodeToDelete = pos.getCurNode();
  if (!nodeToDelete) return;

  Node* replacementNode = nullptr;

  // Case 1: Node has two children
  if (nodeToDelete->left_ && nodeToDelete->right_) {
    // Find the smallest node in right subtree and swap
    replacementNode = find_min(nodeToDelete->right_);
    swapNode(nodeToDelete, replacementNode);
    nodeToDelete = replacementNode;
  }

  // Case 2: Node has at most one child
  if (nodeToDelete->left_) {
    replacementNode = nodeToDelete->left_;
  } else {
    replacementNode = nodeToDelete->right_;
  }

  // Replace nodeToDelete with child (if exists)
  if (replacementNode) {
    replacementNode->parent_ = nodeToDelete->parent_;
    if (nodeToDelete->parent_ == nullptr) {
      root_ = replacementNode;
    } else if (nodeToDelete == nodeToDelete->parent_->left_) {
      nodeToDelete->parent_->left_ = replacementNode;
    } else {
      nodeToDelete->parent_->right_ = replacementNode;
    }

    // If we deleted black, fix the tree
    if (nodeToDelete->color_ == Color::BLACK) {
      fixErase(replacementNode);
    }
  } else if (nodeToDelete->parent_ == nullptr) {
    // Node is the root and has no children
    root_ = nullptr;
  } else {
    // Node is a leaf and has no children
    if (nodeToDelete->color_ == Color::BLACK) {
      fixErase(nodeToDelete);
    }

    if (nodeToDelete->parent_ != nullptr) {
      if (nodeToDelete == nodeToDelete->parent_->left_) {
        nodeToDelete->parent_->left_ = nullptr;
      } else {
        nodeToDelete->parent_->right_ = nullptr;
      }
    }
  }

  delete nodeToDelete;
  --size_;

  // End node: Only update if necessary
  if (root_) {
    Node* newMaxNode = find_max(root_);
    if (endNode_->parent_ != newMaxNode) {
      endNode_->parent_ = newMaxNode;
    }
  }
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::fixErase(Node* node) {
  while (node != root_ && node->color_ == Color::BLACK) {
    if (node == node->parent_->left_) {
      Node* sibling = node->parent_->right_;

      if (sibling->color_ == Color::RED) {
        // Case 1: Sibling is red, rotate
        sibling->color_ = Color::BLACK;
        node->parent_->color_ = Color::RED;
        leftRotate(node->parent_);
        sibling = node->parent_->right_;
      }

      if (sibling->left_ == nullptr || sibling->left_->color_ == Color::BLACK) {
        if (sibling->right_ == nullptr ||
            sibling->right_->color_ == Color::BLACK) {
          // Case 2: Both of sibling's children are black or null
          sibling->color_ = Color::RED;
          node = node->parent_;
        } else {
          if (sibling->right_->color_ == Color::BLACK) {
            // Case 3: Sibling's left child is red, right is black
            sibling->left_->color_ = Color::BLACK;
            sibling->color_ = Color::RED;
            rightRotate(sibling);
            sibling = node->parent_->right_;
          }
          // Case 4: Sibling's right child is red
          sibling->color_ = node->parent_->color_;
          node->parent_->color_ = Color::BLACK;
          if (sibling->right_ != nullptr) {
            sibling->right_->color_ = Color::BLACK;
          }
          leftRotate(node->parent_);
          node = root_;
        }
      } else {
        // Case 5: Sibling's left child is red
        sibling->color_ = node->parent_->color_;
        node->parent_->color_ = Color::BLACK;
        if (sibling->left_ != nullptr) {
          sibling->left_->color_ = Color::BLACK;
        }
        leftRotate(node->parent_);
        node = root_;
      }
    } else {
      // Symmetric to the above
      Node* sibling = node->parent_->left_;

      if (sibling->color_ == Color::RED) {
        // Case 1:
        sibling->color_ = Color::BLACK;
        node->parent_->color_ = Color::RED;
        rightRotate(node->parent_);
        sibling = node->parent_->left_;
      }

      if (sibling->right_ == nullptr ||
          sibling->right_->color_ == Color::BLACK) {
        if (sibling->left_ == nullptr ||
            sibling->left_->color_ == Color::BLACK) {
          // Case 2:
          sibling->color_ = Color::RED;
          node = node->parent_;
        } else {
          if (sibling->left_->color_ == Color::BLACK) {
            // Case 3:
            sibling->right_->color_ = Color::BLACK;
            sibling->color_ = Color::RED;
            leftRotate(sibling);
            sibling = node->parent_->left_;
          }

          // Case 4:
          sibling->color_ = node->parent_->color_;
          node->parent_->color_ = Color::BLACK;
          if (sibling->left_ != nullptr) {
            sibling->left_->color_ = Color::BLACK;
          }
          rightRotate(node->parent_);
          node = root_;
        }
      } else {
        // Case 5:
        sibling->color_ = node->parent_->color_;
        node->parent_->color_ = Color::BLACK;
        if (sibling->right_ != nullptr) {
          sibling->right_->color_ = Color::BLACK;
        }
        rightRotate(node->parent_);
        node = root_;
      }
    }

    if (node != nullptr && node == endNode_) {
      node->parent_ = find_max(root_);
    }
  }

  node->color_ = Color::BLACK;
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::leftRotate(Node* node) {
  Node* rightChild = node->right_;
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
  endNode_->parent_ = find_max(root_);
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
  endNode_->parent_ = find_max(root_);
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::clear() {
  deleteTree(root_);
  root_ = nullptr;
  endNode_->parent_ = nullptr;
  size_ = 0;
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::printTree() const {
  printTree(root_, 0);
}

// --- Private

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::value_type&
RBTree<T, Comparator>::getCurrentValue() const {
  if (!current_node_) {
    throw std::runtime_error("Dereferencing a null iterator");
  }
  return current_node_->value_;
}

template <typename T, typename Comparator>
typename RBTree<T, Comparator>::Node* RBTree<T, Comparator>::copyTree(
    Node* otherNode, Node* parentNode) {
  if (!otherNode) return nullptr;
  Node* newNode = new Node(otherNode->value_, parentNode, otherNode->color_);
  newNode->left_ = copyTree(otherNode->left_, newNode);
  newNode->right_ = copyTree(otherNode->right_, newNode);

  return newNode;
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::swapNode(Node* fNode, Node* sNode) {
  if (!fNode || !sNode) return;
  std::swap(fNode->value_, sNode->value_);
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
  if (node) {
    deleteTree(node->left_);
    deleteTree(node->right_);
    delete node;
  }
}

template <typename T, typename Comparator>
void RBTree<T, Comparator>::printTree(Node* node, int level) const {
  if (node == nullptr) {
    return;
  }

  // Print right subtree first
  printTree(node->right_, level + 1);

  // Print the current node
  for (int i = 0; i < level; ++i) {
    std::cout << "   ";
  }
  std::cout << (node->color_ == Color::RED ? "R " : "B ") << node->value_
            << std::endl;

  // Print left subtree
  printTree(node->left_, level + 1);
}

}  // namespace s21

#endif  // S21_RBTREE
