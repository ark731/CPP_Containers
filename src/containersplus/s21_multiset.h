#ifndef S21_MULTISET_H
#define S21_MULTISET_H

#include "../RBTree/s21_rbtree.h"

namespace s21 {
template <typename Key, typename Comparator = std::less<Key>>
class Multiset {
 public:
  using key_type = Key;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Comparator;
  using value_compare = key_compare;
  using reference = key_type&;
  using const_reference = const key_type&;
  using iterator = typename RBTree<key_type, key_compare>::iterator;
  using const_iterator = typename RBTree<key_type, key_compare>::const_iterator;
  using Node = RBTreeNode<key_type>;

  Multiset() : comp_(), tree_(comp_) {}
  explicit Multiset(const key_compare& comp) : comp_(comp), tree_(comp) {}
  Multiset(const Multiset& other) : comp_(other.comp_), tree_(other.tree_) {}
  Multiset(Multiset&& other) noexcept
      : comp_(std::move(other.comp_)), tree_(std::move(other.tree_)) {}
  ~Multiset() = default;
  Multiset(std::initializer_list<key_type> init_list) : comp_(), tree_(comp_) {
    for (const auto& key : init_list) {
      this->insert(key);
    }
  }
  Multiset(std::initializer_list<key_type> init_list, const key_compare& comp)
      : comp_(comp), tree_(comp_) {
    for (const auto& key : init_list) {
      this->insert(key);
    }
  }

  Multiset& operator=(Multiset other) noexcept {
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

  iterator insert(const key_type& key) {
    return tree_.insertNonUniq(key).first;
  }

  template <typename... Args>
  std::vector<iterator> insert_many(Args&&... args) {
    static_assert((std::is_constructible_v<key_type, Args&&> && ...),
                  "All arguments must be constructible to key_type");
    std::vector<iterator> result;
    result.reserve(sizeof...(args));
    std::vector<iterator> inserted_iterators;
    try {
      (
          [&] {
            auto [it, inserted] = tree_.insertNonUniq(std::forward<Args>(args));
            result.push_back(it);
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

  void merge(Multiset& other) {
    if (this == &other) return;
    tree_.mergeNonUniq(other.tree_);
  }

  void erase(iterator pos) { tree_.erase(pos); }

  size_type erase(const key_type& key) {
    size_type count = 0;
    auto it = find(key);
    if (it != end() && *it == key) {
      erase(it);
      ++count;
    }
    return count;
  }

  size_type erase_all(const key_type& key) {
    size_type count = 0;
    auto it = begin();
    while (it != end()) {
      if (*it == key) {
        auto toErase = it++;
        erase(toErase);
        ++count;
      } else {
        ++it;
      }
    }
    return count;
  }

  void clear() noexcept { tree_.clear(); }
  void swap(Multiset& other) noexcept { tree_.swap(other.tree_); }

  size_type count(const key_type& key) const {
    auto range = equal_range(key);
    return std::distance(range.first, range.second);
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

  key_compare key_comp() const { return comp_; }
  void printTree() const { tree_.printTree(); }

 private:
  key_compare comp_;
  RBTree<key_type, key_compare> tree_;
};

}  // namespace s21

#endif  // S21_MULTISET_H
