#ifndef S21_SET_H
#define S21_SET_H

#include "../RBTree/s21_rbtree.h"

namespace s21 {

template <typename Key, typename Comparator = std::less<Key>>
class Set {
 public:
  using key_type = Key;
  using key_compare = Comparator;
  using value_compare = key_compare;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = key_type&;
  using const_reference = const key_type&;
  using iterator = typename RBTree<key_type, key_compare>::iterator;
  using const_iterator = typename RBTree<key_type, key_compare>::const_iterator;
  using Node = RBTreeNode<key_type>;

  Set() : tree_() {}
  explicit Set(const key_compare& comp) : tree_(comp) {}
  Set(const Set& other) : tree_(other.tree_) {}
  Set(Set&& other) noexcept : tree_(std::move(other.tree_)) {}
  ~Set() = default;
  Set(std::initializer_list<key_type> init_list) : comp_(), tree_(comp_) {
    for (const auto& key : init_list) {
      this->insert(key);
    }
  }
  Set(std::initializer_list<key_type> init_list, const key_compare& comp)
      : comp_(comp), tree_(comp_) {
    for (const auto& key : init_list) {
      this->insert(key);
    }
  }

  Set& operator=(Set other) noexcept {
    swap(other);
    return *this;
  }

  iterator begin() { return tree_.begin(); }
  const_iterator begin() const { return tree_.begin(); }
  iterator end() { return tree_.end(); }
  const_iterator end() const { return tree_.end(); }

  bool empty() const noexcept { return tree_.empty(); }
  size_type size() const noexcept { return tree_.size(); }
  size_type max_size() const noexcept { return tree_.max_size(); }

  std::pair<iterator, bool> insert(const key_type& key) {
    std::vector<iterator> rollback_nodes;
    try {
      auto [it, inserted] = tree_.insert(key);
      if (inserted) {
        rollback_nodes.push_back(it);
      }
      return {it, inserted};
    } catch (...) {
      for (auto it : rollback_nodes) {
        erase(it);
      }
      throw;
    }
  }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    static_assert((std::is_constructible_v<key_type, Args&&> && ...),
                  "All arguments must be constructible to key_type");
    std::vector<std::pair<iterator, bool>> result;
    result.reserve(sizeof...(args));
    std::vector<iterator> inserted_iterators;
    try {
      (
          [&] {
            auto [it, inserted] = insert(std::forward<Args>(args));
            result.push_back({it, inserted});
            if (inserted) {
              inserted_iterators.push_back(it);
            }
          }(),
          ...);
    } catch (...) {
      for (auto it : inserted_iterators) {
        erase(it);
      }
      throw;
    }
    return result;
  }

  void merge(Set& other) {
    if (this == &other) return;
    tree_.merge(other.tree_);
  }

  void erase(iterator pos) { tree_.erase(pos); }
  size_type erase(const key_type& key) {
    auto it = find(key);
    if (it != end()) {
      tree_.erase(it);
      return 1;
    }
    return 0;
  }
  void clear() noexcept { tree_.clear(); }
  void swap(Set& other) noexcept { tree_.swap(other.tree_); }

  size_type count(const key_type& key) const {
    return find(key) != end() ? 1 : 0;
  }

  const_iterator find(const key_type& key) const {
    auto it = tree_.find(key);
    return it != tree_.end() ? it : end();
  }

  const_iterator lower_bound(const key_type& key) const {
    auto it = tree_.lower_bound(key);
    return it != tree_.end() ? it : end();
  }

  const_iterator upper_bound(const key_type& key) const {
    auto it = tree_.upper_bound(key);
    return it != tree_.end() ? it : end();
  }

  std::pair<const_iterator, const_iterator> equal_range(
      const key_type& key) const {
    return {lower_bound(key), upper_bound(key)};
  }

  iterator find(const key_type& key) {
    auto it = tree_.find(key);
    return it != tree_.end() ? it : end();
  }

  iterator lower_bound(const key_type& key) {
    auto it = tree_.lower_bound(key);
    return it != tree_.end() ? it : end();
  }

  iterator upper_bound(const key_type& key) {
    auto it = tree_.upper_bound(key);
    return it != tree_.end() ? it : end();
  }

  std::pair<iterator, iterator> equal_range(const key_type& key) {
    return {lower_bound(key), upper_bound(key)};
  }

  Comparator key_comp() const { return comp_; }

  void printTree() const { tree_.printTree(); }

 private:
  Comparator comp_;
  RBTree<key_type, Comparator> tree_;
};

}  // namespace s21

#endif  // S21_SET_H
