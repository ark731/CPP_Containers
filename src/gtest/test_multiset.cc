#include <gtest/gtest.h>

#include <utility>

#include "../s21_containersplus.h"

namespace s21 {

class MultisetTest : public ::testing::Test {
 protected:
  Multiset<int> default_multiset;
  Multiset<int, std::greater<int>> custom_comp_multiset{std::greater<int>()};
};

// Test default constructor
TEST_F(MultisetTest, DefaultConstructor) {
  EXPECT_TRUE(default_multiset.empty());
  EXPECT_EQ(default_multiset.size(), 0);
}

// Test constructor with custom comparator
TEST_F(MultisetTest, CustomComparatorConstructor) {
  EXPECT_TRUE(custom_comp_multiset.empty());
  EXPECT_EQ(custom_comp_multiset.size(), 0);
}

// Test insert single element
TEST_F(MultisetTest, InsertSingleElement) {
  auto it = default_multiset.insert(10);
  EXPECT_FALSE(default_multiset.empty());
  EXPECT_EQ(default_multiset.size(), 1);
  EXPECT_EQ(*it, 10);
}

// Test insert duplicate elements
TEST_F(MultisetTest, InsertDuplicateElements) {
  default_multiset.insert(10);
  default_multiset.insert(10);
  EXPECT_EQ(default_multiset.size(), 2);

  auto it = default_multiset.begin();
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(*it, 10);
}

// Test custom comparator behavior
TEST_F(MultisetTest, CustomComparatorInsert) {
  custom_comp_multiset.insert(5);
  custom_comp_multiset.insert(10);
  custom_comp_multiset.insert(1);

  auto it = custom_comp_multiset.begin();
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(*it, 5);
  ++it;
  EXPECT_EQ(*it, 1);
}

// Test empty() method
TEST_F(MultisetTest, EmptyMethod) {
  EXPECT_TRUE(default_multiset.empty());
  default_multiset.insert(42);
  EXPECT_FALSE(default_multiset.empty());
}

// Test clear() method
TEST_F(MultisetTest, ClearMethod) {
  default_multiset.insert(10);
  default_multiset.insert(20);
  EXPECT_EQ(default_multiset.size(), 2);

  default_multiset.clear();
  EXPECT_TRUE(default_multiset.empty());
  EXPECT_EQ(default_multiset.size(), 0);
}

// Test size() method
TEST_F(MultisetTest, SizeMethod) {
  EXPECT_EQ(default_multiset.size(), 0);
  default_multiset.insert(10);
  EXPECT_EQ(default_multiset.size(), 1);
  default_multiset.insert(20);
  EXPECT_EQ(default_multiset.size(), 2);
}

// Test max_size() method
TEST_F(MultisetTest, MaxSizeMethod) {
  EXPECT_GT(default_multiset.max_size(), 0);
}

// Test iterator begin() and end()
TEST_F(MultisetTest, IteratorBeginEnd) {
  default_multiset.insert(10);
  default_multiset.insert(20);

  auto it = default_multiset.begin();
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(*it, 20);
  ++it;
  EXPECT_EQ(it, default_multiset.end());
}

// Test erase by iterator
TEST_F(MultisetTest, EraseByIterator) {
  default_multiset.insert(10);
  default_multiset.insert(20);
  default_multiset.insert(30);
  EXPECT_EQ(default_multiset.size(), 3);

  auto it = default_multiset.begin();
  ++it;  // Move to 20
  default_multiset.erase(it);

  EXPECT_EQ(default_multiset.size(), 2);
  it = default_multiset.begin();
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(*it, 30);
}

// Test erase by key
TEST_F(MultisetTest, EraseByKey) {
  default_multiset.insert(10);
  default_multiset.insert(20);
  default_multiset.insert(10);
  EXPECT_EQ(default_multiset.size(), 3);

  size_t erased = default_multiset.erase(10);
  EXPECT_EQ(erased, 1);
  EXPECT_EQ(default_multiset.size(), 2);
  erased = default_multiset.erase(10);  // Try to erase 10 again
  EXPECT_EQ(erased, 1);
  EXPECT_EQ(default_multiset.size(), 1);
}

// Test erase by key
TEST_F(MultisetTest, EraseAllByKey) {
  default_multiset.insert(10);
  default_multiset.insert(20);
  default_multiset.insert(10);
  EXPECT_EQ(default_multiset.size(), 3);

  size_t erased = default_multiset.erase_all(10);
  EXPECT_EQ(erased, 2);
  EXPECT_EQ(default_multiset.size(), 1);
  erased = default_multiset.erase(10);  // Try to erase 10 again
  EXPECT_EQ(erased, 0);
  EXPECT_EQ(default_multiset.size(), 1);
}

// Test erase all occurrences of a key
TEST_F(MultisetTest, EraseAllOccurrences) {
  default_multiset.insert(10);
  default_multiset.insert(10);
  default_multiset.insert(10);
  EXPECT_EQ(default_multiset.size(), 3);

  size_t erased = default_multiset.erase_all(10);
  EXPECT_EQ(erased, 3);
  EXPECT_EQ(default_multiset.size(), 0);
}

// Test count method
TEST_F(MultisetTest, CountMethod) {
  default_multiset.insert(10);
  default_multiset.insert(20);
  default_multiset.insert(10);

  EXPECT_EQ(default_multiset.count(10), 2);  // Count of 10
  EXPECT_EQ(default_multiset.count(20), 1);  // Count of 20
  EXPECT_EQ(default_multiset.count(30), 0);  // Count of 30 (not inserted)
}

// Test insert_many with multiple arguments
TEST_F(MultisetTest, InsertMany) {
  auto result = default_multiset.insert_many(10, 20, 30, 20, 10);
  EXPECT_EQ(result.size(), 5);            // 5 insertions attempted
  EXPECT_EQ(default_multiset.size(), 5);  // 5 elements in multiset

  EXPECT_EQ(default_multiset.count(10), 2);
  EXPECT_EQ(default_multiset.count(20), 2);
  EXPECT_EQ(default_multiset.count(30), 1);
}

// Test insert_many with empty input
TEST_F(MultisetTest, InsertManyEmptyInput) {
  auto result = default_multiset.insert_many();
  EXPECT_EQ(result.size(), 0);  // No elements inserted
  EXPECT_TRUE(default_multiset.empty());
}

// Test custom comparator with insert_many
TEST_F(MultisetTest, CustomComparatorInsertMany) {
  auto result = custom_comp_multiset.insert_many(10, 20, 30, 20, 10);
  EXPECT_EQ(result.size(), 5);
  EXPECT_EQ(custom_comp_multiset.size(), 5);

  auto it = custom_comp_multiset.begin();
  EXPECT_EQ(*it, 30);  // Largest element first due to greater comparator
  ++it;
  EXPECT_EQ(*it, 20);
  ++it;
  EXPECT_EQ(*it, 20);
  ++it;
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(*it, 10);
}

// Test swap() method
TEST_F(MultisetTest, SwapMethod) {
  default_multiset.insert(10);
  default_multiset.insert(20);

  Multiset<int> other_multiset;
  other_multiset.insert(30);
  other_multiset.insert(40);

  EXPECT_EQ(default_multiset.size(), 2);
  EXPECT_EQ(other_multiset.size(), 2);

  default_multiset.swap(other_multiset);

  EXPECT_EQ(default_multiset.size(), 2);
  EXPECT_EQ(other_multiset.size(), 2);

  auto it = default_multiset.begin();
  EXPECT_EQ(*it, 30);
  ++it;
  EXPECT_EQ(*it, 40);

  it = other_multiset.begin();
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(*it, 20);
}

// Test key_comp() method
TEST_F(MultisetTest, KeyComparator) {
  auto comp = default_multiset.key_comp();
  EXPECT_TRUE(comp(1, 2));  // Default comparator is std::less

  auto custom_comp = custom_comp_multiset.key_comp();
  EXPECT_TRUE(custom_comp(2, 1));  // Custom comparator is std::greater
}

// Test upper_bound() and lower_bound() methods
TEST_F(MultisetTest, BoundMethods) {
  default_multiset.insert(10);
  default_multiset.insert(20);
  default_multiset.insert(20);
  default_multiset.insert(30);

  auto lb = default_multiset.lower_bound(20);
  auto ub = default_multiset.upper_bound(20);

  EXPECT_EQ(*lb, 20);
  ++lb;
  EXPECT_EQ(*lb, 20);
  EXPECT_EQ(*ub, 30);
}

// Test equal_range() method
TEST_F(MultisetTest, EqualRange) {
  default_multiset.insert(10);
  default_multiset.insert(20);
  default_multiset.insert(20);
  default_multiset.insert(30);

  auto range = default_multiset.equal_range(20);
  auto it = range.first;

  EXPECT_EQ(*it, 20);
  ++it;
  EXPECT_EQ(*it, 20);
  ++it;
  EXPECT_EQ(it, range.second);
}

// Test copy constructor
TEST_F(MultisetTest, CopyConstructor) {
  default_multiset.insert(10);
  default_multiset.insert(20);

  Multiset<int> copied_multiset(default_multiset);

  EXPECT_EQ(copied_multiset.size(), default_multiset.size());
  auto it1 = default_multiset.begin();
  auto it2 = copied_multiset.begin();
  while (it1 != default_multiset.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1;
    ++it2;
  }
}

// Test move constructor
TEST_F(MultisetTest, MoveConstructor) {
  default_multiset.insert(10);
  default_multiset.insert(20);

  Multiset<int> moved_multiset(std::move(default_multiset));

  EXPECT_TRUE(default_multiset.empty());
  EXPECT_EQ(moved_multiset.size(), 2);

  auto it = moved_multiset.begin();
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(*it, 20);
}

// Test copy assignment operator
TEST_F(MultisetTest, CopyAssignment) {
  default_multiset.insert(10);
  default_multiset.insert(20);

  Multiset<int> assigned_multiset;
  assigned_multiset = default_multiset;

  EXPECT_EQ(assigned_multiset.size(), default_multiset.size());
  auto it1 = default_multiset.begin();
  auto it2 = assigned_multiset.begin();
  while (it1 != default_multiset.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1;
    ++it2;
  }
}

// Test move assignment operator
TEST_F(MultisetTest, MoveAssignment) {
  default_multiset.insert(10);
  default_multiset.insert(20);

  Multiset<int> assigned_multiset;
  assigned_multiset = std::move(default_multiset);

  EXPECT_TRUE(default_multiset.empty());
  EXPECT_EQ(assigned_multiset.size(), 2);

  auto it = assigned_multiset.begin();
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(*it, 20);
}

// Test insert and iterator stability
TEST_F(MultisetTest, IteratorStability) {
  default_multiset.insert(10);
  default_multiset.insert(20);
  default_multiset.insert(30);

  auto it = default_multiset.begin();
  ++it;  // Pointing to 20

  default_multiset.insert(25);  // Inserting new element

  EXPECT_EQ(*it, 20);  // Iterator should still point to 20
}

// Test edge case: inserting into an empty multiset
TEST_F(MultisetTest, InsertIntoEmpty) {
  auto it = default_multiset.insert(42);
  EXPECT_EQ(*it, 42);
  EXPECT_EQ(default_multiset.size(), 1);
}

// Test edge case: erasing from an empty multiset
TEST_F(MultisetTest, EraseFromEmpty) {
  EXPECT_NO_THROW(default_multiset.erase(default_multiset.begin()));
  EXPECT_EQ(default_multiset.size(), 0);
}

// Test edge case: count on an empty multiset
TEST_F(MultisetTest, CountOnEmpty) { EXPECT_EQ(default_multiset.count(42), 0); }

}  // namespace s21
