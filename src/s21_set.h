#ifndef S21_SET_H
#define S21_SET_H

#include "s21_rbtree.h"

namespace s21 {

template <typename Key, typename Comparator = std::less<Key>>
class Set {
 public:
  using key_type = Key;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Comparator;
  using reference = key_type&;
  using const_reference = const key_type&;
  using iterator = Iterator<key_type>;
  using const_iterator = ConstIterator<key_type>;
  using Node = RBTreeNode<key_type>;

  // Constructors and Destructor
  Set() : comp_(), tree_(comp_) {}
  explicit Set(const key_compare& comp) : comp_(comp), tree_(comp) {}
  Set(const Set& other) : comp_(other.comp_), tree_(other.tree_) {}
  Set(Set&& other) noexcept
      : comp_(std::move(other.comp_)), tree_(std::move(other.tree_)) {}
  ~Set() = default;

  Set& operator=(Set other) noexcept {
    swap(other);
    return *this;
  }

  // Iterators
  iterator begin() { return tree_.begin(); }
  const_iterator begin() const { return tree_.begin(); }
  iterator end() { return tree_.end(); }
  const_iterator end() const { return tree_.end(); }

  // Capacity
  bool empty() const noexcept { return tree_.empty(); }
  size_type size() const noexcept { return tree_.size(); }
  size_type max_size() const noexcept { return tree_.max_size(); }

  // Modifiers
  std::pair<iterator, bool> insert(const key_type& key) {
    return tree_.insert(key);
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
  void swap(Set& other) noexcept {
    std::swap(comp_, other.comp_);
    tree_.swap(other.tree_);
  }

  // Lookup
  iterator find(const key_type& key) const {
    auto it = tree_.find(key);
    return it != tree_.end() ? it : end();
  }
  size_type count(const key_type& key) const {
    return find(key) != end() ? 1 : 0;
  }
  iterator lower_bound(const key_type& key) const {
    auto it = tree_.lower_bound(key);
    return it != tree_.end() ? it : end();
  }
  iterator upper_bound(const key_type& key) const {
    auto it = tree_.upper_bound(key);
    return it != tree_.end() ? it : end();
  }
  std::pair<iterator, iterator> equal_range(const key_type& key) const {
    return {lower_bound(key), upper_bound(key)};
  }

  // Observers
  key_compare key_comp() const { return comp_; }

 private:
  key_compare comp_;
  RBTree<key_type, key_compare> tree_;
};

}  // namespace s21

#endif  // S21_SET_H
