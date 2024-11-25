#ifndef S21_MAP_H
#define S21_MAP_H

#include "../RBTree/s21_rbtree.h"

namespace s21 {

template <typename Key, typename T, typename Comparator = std::less<Key>>
struct ValueComparator {
  bool operator()(const std::pair<const Key, T>& val1,
                  const std::pair<const Key, T>& val2) const {
    return Comparator()(val1.first, val2.first);
  }
};

template <typename Key, typename T, typename Comparator = std::less<Key>>
class Map {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Comparator;
  using value_compare = ValueComparator<Key, T, Comparator>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = Iterator<value_type>;
  using const_iterator = ConstIterator<value_type>;
  using Node = RBTreeNode<value_type>;

  // Constructors and Destructor
  Map() : comp_(), tree_(value_compare(comp_)) {}
  explicit Map(const key_compare& comp)
      : comp_(comp), tree_(value_compare(comp)) {}
  Map(const Map& other) : comp_(other.comp_), tree_(other.tree_) {}
  Map(Map&& other) noexcept
      : comp_(std::move(other.comp_)), tree_(std::move(other.tree_)) {}
  ~Map() = default;
  Map(std::initializer_list<std::pair<const Key, T>> init_list)
      : comp_(), tree_(value_compare(comp_)) {
    for (const auto& pair : init_list) {
      this->insert(pair);
    }
  }

  Map(std::initializer_list<std::pair<const Key, T>> init_list,
      const key_compare& comp)
      : comp_(comp), tree_(value_compare(comp_)) {
    for (const auto& pair : init_list) {
      this->insert(pair);
    }
  }

  Map& operator=(
      Map other) noexcept {  // Copy-and-swap idiom for exception safety
    swap(other);
    return *this;
  }

  // Iterators
  iterator begin() { return tree_.begin(); }
  iterator end() { return tree_.end(); }
  const_iterator begin() const { return tree_.begin(); }
  const_iterator end() const { return tree_.end(); }

  // Capacity
  bool empty() const noexcept { return tree_.empty(); }
  size_type size() const noexcept { return tree_.size(); }
  size_type max_size() const noexcept { return tree_.max_size(); }

  // Modifiers
  std::pair<iterator, bool> insert(const value_type& value) {
    return tree_.insert(value);
  }
  iterator insert(iterator hint, const value_type& value) {
    return tree_.insert(hint, value);
  }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    static_assert((std::is_constructible_v<value_type, Args&&> && ...),
                  "All arguments must be constructible to value_type");

    std::vector<std::pair<iterator, bool>> result;
    result.reserve(sizeof...(args));  // Reserve space for efficiency

    // Track successful insertions to rollback in case of failure
    std::vector<iterator> inserted_iterators;

    try {
      // Iterate over each argument and insert
      (
          [&] {
            auto [it, inserted] = insert(std::forward<Args>(args));
            result.push_back({it, inserted});
            if (inserted) {
              inserted_iterators.push_back(it);  // Track successful insertions
            }
          }(),
          ...);

    } catch (...) {
      // Rollback all successful insertions to maintain consistency
      for (auto it : inserted_iterators) {
        erase(it);  // Assuming erase handles rolling back the inserted node.
      }
      throw;  // Re-throw the exception to propagate
    }

    return result;
  }

  void erase(iterator pos) { tree_.erase(pos); }
  size_type erase(const key_type& key) {
    auto it = find(key);
    if (it != end()) {  // Ensure we're not at end before erasing
      tree_.erase(it);
      return 1;
    }
    return 0;
  }
  void clear() noexcept { tree_.clear(); }
  void swap(Map& other) noexcept {
    std::swap(comp_, other.comp_);
    tree_.swap(other.tree_);
  }

  // Element Access
  T& at(const key_type& key) {
    auto it = find(key);
    if (it == end()) throw std::out_of_range("Key not found");
    return it->second;
  }
  const T& at(const key_type& key) const {
    auto it = find(key);
    if (it == end()) throw std::out_of_range("Key not found");
    return it->second;
  }
  T& operator[](const key_type& key) {
    return insert(std::make_pair(key, T())).first->second;
  }
  T& operator[](key_type&& key) {
    return insert(std::make_pair(std::move(key), T())).first->second;
  }

  // Lookup
  iterator find(const key_type& key) {
    auto it = tree_.find(value_type(key, T()));
    return it != tree_.end() ? it : end();
  }
  const_iterator find(const key_type& key) const {
    auto it = tree_.find(value_type(key, T()));
    return it != tree_.end() ? it : const_iterator(end());
  }
  size_type count(const key_type& key) const {
    return find(key) != end() ? 1 : 0;
  }
  iterator lower_bound(const key_type& key) {
    auto it = tree_.lower_bound(value_type(key, T()));
    return it != tree_.end() ? it : end();
  }
  iterator upper_bound(const key_type& key) {
    auto it = tree_.upper_bound(value_type(key, T()));
    return it != tree_.end() ? it : end();
  }
  const_iterator lower_bound(const key_type& key) const {
    auto it = tree_.lower_bound(value_type(key, T()));
    return it != tree_.end() ? it : const_iterator(end());
  }
  const_iterator upper_bound(const key_type& key) const {
    auto it = tree_.upper_bound(value_type(key, T()));
    return it != tree_.end() ? it : const_iterator(end());
  }
  std::pair<iterator, iterator> equal_range(const key_type& key) {
    return {lower_bound(key), upper_bound(key)};
  }

  // Observers
  key_compare key_comp() const { return comp_; }
  value_compare value_comp() const {
    return value_compare(comp_);
  }  // Provides access to ValueComparator

 private:
  key_compare comp_;
  RBTree<value_type, value_compare> tree_;
};

}  // namespace s21

#endif  // S21_MAP_H
