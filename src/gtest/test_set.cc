#include <gtest/gtest.h>

#include <utility>

#include "../s21_containers.h"

namespace s21 {

class SetTest : public ::testing::Test {
 protected:
  Set<int> default_set;
  Set<int, std::greater<int>> custom_comp_set{std::greater<int>()};
};

// Test default constructor
TEST_F(SetTest, DefaultConstructor) {
  EXPECT_TRUE(default_set.empty());
  EXPECT_EQ(default_set.size(), 0);
}

// Test constructor with custom comparator
TEST_F(SetTest, CustomComparatorConstructor) {
  EXPECT_TRUE(custom_comp_set.empty());
  EXPECT_EQ(custom_comp_set.size(), 0);
}

// Test copy constructor
TEST_F(SetTest, CopyConstructor) {
  default_set.insert(10);
  default_set.insert(20);

  Set<int> copied_set = default_set;
  EXPECT_EQ(copied_set.size(), 2);
  EXPECT_TRUE(copied_set.count(10));
  EXPECT_TRUE(copied_set.count(20));
}

// Test move constructor
TEST_F(SetTest, MoveConstructor) {
  default_set.insert(10);
  default_set.insert(20);

  Set<int> moved_set = std::move(default_set);
  EXPECT_EQ(moved_set.size(), 2);
  EXPECT_TRUE(moved_set.count(10));
  EXPECT_TRUE(moved_set.count(20));
  EXPECT_TRUE(default_set.empty());  // Default set should be empty after move
}

// Test assignment operator
TEST_F(SetTest, AssignmentOperator) {
  default_set.insert(10);
  default_set.insert(20);

  Set<int> assigned_set;
  assigned_set = default_set;

  EXPECT_EQ(assigned_set.size(), 2);
  EXPECT_TRUE(assigned_set.count(10));
  EXPECT_TRUE(assigned_set.count(20));
}

// Test insert method
TEST_F(SetTest, Insert) {
  auto [it, inserted] = default_set.insert(30);
  EXPECT_TRUE(inserted);  // Should be inserted
  EXPECT_EQ(default_set.size(), 1);
  EXPECT_TRUE(default_set.count(30));

  // Insert a duplicate, should not be inserted
  auto [it_dup, inserted_dup] = default_set.insert(30);
  EXPECT_FALSE(inserted_dup);        // Should not be inserted
  EXPECT_EQ(default_set.size(), 1);  // Size should still be 1
}

// Test insert_many method
TEST_F(SetTest, InsertMany) {
  auto results = default_set.insert_many(1, 2, 3, 4, 5, 6, 7, 8);
  EXPECT_EQ(default_set.size(), 8);
  for (int i = 1; i <= 8; ++i) {
    EXPECT_TRUE(default_set.count(i));
  }
}

// Test insert_many method
TEST_F(SetTest, InsertMany2) {
  default_set.insert(10);
  default_set.insert(20);
  default_set.insert_many(1, 2, 3, 4, 5, 6, 7, 8);
  EXPECT_EQ(default_set.size(), 10);
  for (int i = 1; i <= 8; ++i) {
    EXPECT_TRUE(default_set.count(i));
  }
  default_set.insert_many(10, 20, 30, 40, 50);
  EXPECT_EQ(default_set.size(), 13);
  for (int i = 10; i <= 50; i += 10) {
    EXPECT_TRUE(default_set.count(i));
  }
}

// Test insert_many method
TEST_F(SetTest, InsertMany3) {
  default_set.insert(10);
  default_set.insert(20);
  default_set.insert_many(17, 23, 3, 4, 5, 6, 17, 8);
  default_set.insert_many(10, 20, 1, 30, 40, 50);
  default_set.insert_many(423, 9, 16, 150, 11, 32, 2, 33, 7);
  EXPECT_EQ(default_set.size(), 22);
}

// Test erase method
TEST_F(SetTest, Erase) {
  default_set.insert(10);
  default_set.insert(20);

  auto it = default_set.find(10);
  default_set.erase(it);

  EXPECT_EQ(default_set.size(), 1);
  EXPECT_FALSE(default_set.count(10));
  EXPECT_TRUE(default_set.count(20));

  // Erase non-existent element
  size_t erase_result = default_set.erase(30);
  EXPECT_EQ(erase_result, 0);  // No elements were erased
}

// Test erase method
TEST_F(SetTest, Erase2) {
  default_set.insert(10);
  default_set.insert(20);
  default_set.insert_many(1, 2, 3, 4, 5, 6, 7, 8);
  EXPECT_EQ(default_set.size(), 10);
  for (int i = 1; i <= 8; ++i) {
    EXPECT_TRUE(default_set.count(i));
  }
  default_set.insert_many(10, 20, 30, 40, 50);
  for (int i = 1; i <= 8; ++i) {
    EXPECT_TRUE(default_set.count(i));
  }

  auto it = default_set.find(10);
  default_set.erase(it);

  it = default_set.find(6);
  default_set.erase(it);

  EXPECT_FALSE(default_set.count(10));
  EXPECT_FALSE(default_set.count(6));

  // Erase non-existent element
  size_t erase_result = default_set.erase(300);
  EXPECT_EQ(erase_result, 0);  // No elements were erased
}

// Test clear method
TEST_F(SetTest, Clear) {
  default_set.insert(10);
  default_set.insert(20);

  default_set.clear();

  EXPECT_TRUE(default_set.empty());
  EXPECT_EQ(default_set.size(), 0);
}

// Test custom comparator: std::greater<int>
TEST_F(SetTest, CustomComparatorOrder) {
  custom_comp_set.insert(10);
  custom_comp_set.insert(20);
  custom_comp_set.insert(30);

  // The expected order should be 30, 20, 10 for std::greater<int>
  auto it = custom_comp_set.begin();
  EXPECT_EQ(*it, 30);

  it = std::next(it);
  EXPECT_EQ(*it, 20);

  it = std::next(it);
  EXPECT_EQ(*it, 10);
}

// Test iterators: begin and end
TEST_F(SetTest, Iterators) {
  default_set.insert(10);
  default_set.insert(20);
  default_set.insert(30);

  auto it = default_set.begin();
  EXPECT_EQ(*it, 10);  // Should be the smallest element

  it = std::next(it);
  EXPECT_EQ(*it, 20);

  it = std::next(it);
  EXPECT_EQ(*it, 30);

  // Make sure next is handled correctly at the end of the container
  it = std::next(it);
  EXPECT_EQ(it, default_set.end());  // After last element, next should be end()
}

TEST_F(SetTest, StressTestInsert) {
  for (int i = 0; i < 100000; ++i) {
    default_set.insert(i);
  }
  EXPECT_EQ(default_set.size(), 100000);
}

TEST_F(SetTest, StressTestErase) {
  for (int i = 0; i < 100000; ++i) {
    default_set.insert(i);
  }
  for (int i = 0; i < 50000; ++i) {
    auto it = default_set.find(i);
    default_set.erase(it);
  }
  EXPECT_EQ(default_set.size(), 50000);
}

TEST_F(SetTest, ComparatorRespected) {
  custom_comp_set.insert(5);
  custom_comp_set.insert(1);
  custom_comp_set.insert(10);

  auto it = custom_comp_set.begin();
  EXPECT_EQ(*it, 10);  // std::greater orders in descending
  ++it;
  EXPECT_EQ(*it, 5);
  ++it;
  EXPECT_EQ(*it, 1);
}

TEST_F(SetTest, Merge) {
  default_set.insert(1);
  default_set.insert(2);

  Set<int> other_set;
  other_set.insert(3);
  other_set.insert(1);  // Duplicate

  default_set.merge(other_set);

  EXPECT_EQ(default_set.size(), 3);
  EXPECT_TRUE(default_set.count(1));
  EXPECT_TRUE(default_set.count(2));
  EXPECT_TRUE(default_set.count(3));
}

TEST_F(SetTest, BasicMerge) {
  default_set.insert(1);
  default_set.insert(2);

  Set<int> other_set;
  other_set.insert(3);
  other_set.insert(4);

  default_set.merge(other_set);

  EXPECT_EQ(default_set.size(), 4);
  EXPECT_TRUE(default_set.count(1));
  EXPECT_TRUE(default_set.count(2));
  EXPECT_TRUE(default_set.count(3));
  EXPECT_TRUE(default_set.count(4));
  EXPECT_TRUE(other_set.empty());  // Other set should be empty after merge
}

TEST_F(SetTest, MergeWithDuplicates) {
  default_set.insert(1);
  default_set.insert(2);

  Set<int> other_set;
  other_set.insert(2);
  other_set.insert(3);

  default_set.merge(other_set);

  EXPECT_EQ(default_set.size(), 3);
  EXPECT_TRUE(default_set.count(1));
  EXPECT_TRUE(default_set.count(2));
  EXPECT_TRUE(default_set.count(3));
  EXPECT_TRUE(other_set.empty());  // Other set should be empty after merge
}

TEST_F(SetTest, SelfMerge) {
  default_set.insert(1);
  default_set.insert(2);

  default_set.merge(default_set);

  EXPECT_EQ(default_set.size(), 2);
  EXPECT_TRUE(default_set.count(1));
  EXPECT_TRUE(default_set.count(2));
}

TEST_F(SetTest, MergeIntoEmptySet) {
  Set<int> other_set;
  other_set.insert(1);
  other_set.insert(2);

  default_set.merge(other_set);

  EXPECT_EQ(default_set.size(), 2);
  EXPECT_TRUE(default_set.count(1));
  EXPECT_TRUE(default_set.count(2));
  EXPECT_TRUE(other_set.empty());
}

TEST_F(SetTest, ReverseIteration) {
  default_set.insert(1);
  default_set.insert(2);
  default_set.insert(3);

  default_set.printTree();  ////////////////  DELETE

  auto it = default_set.end();
  --it;
  EXPECT_EQ(*it, 3);

  --it;
  EXPECT_EQ(*it, 2);

  --it;
  EXPECT_EQ(*it, 1);
}

TEST_F(SetTest, SingleElement) {
  default_set.insert(42);
  EXPECT_EQ(default_set.size(), 1);
  EXPECT_EQ(*default_set.begin(), 42);
}

TEST_F(SetTest, EmptyFind) {
  EXPECT_EQ(default_set.find(10), default_set.end());
}

struct FailingComparator {
  bool operator()(int lhs, int rhs) const {
    if (lhs == 42 || rhs == 42) throw std::runtime_error("Comparator failure");
    return lhs < rhs;
  }
};

TEST(SetInsert, ExceptionDuringComparator) {
  s21::Set<int, FailingComparator> set;

  // Insert elements that won't cause exceptions
  EXPECT_NO_THROW(set.insert(10));
  EXPECT_NO_THROW(set.insert(20));

  // Attempt to insert an element causing the comparator to throw
  EXPECT_THROW(set.insert(42), std::runtime_error);

  // Ensure the set remains consistent
  EXPECT_EQ(set.size(), 2);
  EXPECT_TRUE(set.count(10));
  EXPECT_TRUE(set.count(20));
}

TEST(SetInsertMany, ExceptionDuringInsertMany) {
  s21::Set<int, FailingComparator> set;

  // Attempt to insert a range of values where one will throw
  EXPECT_THROW(set.insert_many(1, 2, 42, 3, 4), std::runtime_error);

  // Ensure no partial insertion occurred
  EXPECT_TRUE(set.empty());
}

struct FailingKey {
  int value;

  FailingKey() : value(0) {}  // Default constructor
  FailingKey(int v) : value(v) {
    if (v == 42) throw std::runtime_error("Key construction failure");
  }

  bool operator<(const FailingKey& other) const { return value < other.value; }
};

TEST(SetInsert, ExceptionDuringKeyConstruction) {
  s21::Set<FailingKey> set;

  // Insert elements that won't throw
  EXPECT_NO_THROW(set.insert(FailingKey(10)));
  EXPECT_NO_THROW(set.insert(FailingKey(20)));

  // Attempt to insert an element causing key construction to throw
  EXPECT_THROW(set.insert(FailingKey(42)), std::runtime_error);

  // Ensure the set remains consistent
  EXPECT_EQ(set.size(), 2);
}

}  // namespace s21
