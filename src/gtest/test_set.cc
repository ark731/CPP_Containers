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

TEST_F(SetTest, ExceptionDuringComparator) {
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

TEST_F(SetTest, ExceptionDuringInsertMany) {
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

TEST_F(SetTest, ExceptionDuringKeyConstruction) {
  s21::Set<FailingKey> set;

  // Insert elements that won't throw
  EXPECT_NO_THROW(set.insert(FailingKey(10)));
  EXPECT_NO_THROW(set.insert(FailingKey(20)));

  // Attempt to insert an element causing key construction to throw
  EXPECT_THROW(set.insert(FailingKey(42)), std::runtime_error);

  // Ensure the set remains consistent
  EXPECT_EQ(set.size(), 2);
}

TEST_F(SetTest, ReverseTraversalSimpleTree) {
  s21::RBTree<int> tree;
  tree.insert(10);
  tree.insert(20);
  tree.insert(30);

  auto it = tree.find(30);  // Start at max node
  EXPECT_EQ(*it, 30);

  --it;
  EXPECT_EQ(*it, 20);

  --it;
  EXPECT_EQ(*it, 10);

  --it;
  EXPECT_EQ(it, tree.end());  // Moving before the first node should return end
}

TEST_F(SetTest, TreeStructure) {
  s21::RBTree<int> tree;

  tree.insert(10);
  EXPECT_EQ(tree.size(), 1);

  tree.insert(20);
  EXPECT_EQ(tree.size(), 2);

  tree.insert(5);
  EXPECT_EQ(tree.size(), 3);

  auto it = tree.begin();
  EXPECT_EQ(*it, 5);  // Minimum value
}

TEST_F(SetTest, Boundaries) {
  s21::RBTree<int> tree;

  tree.insert(10);
  tree.insert(20);
  tree.insert(30);

  auto it = tree.begin();
  EXPECT_EQ(*it, 10);  // Minimum node

  --it;
  EXPECT_EQ(it, tree.end());  // Moving before the first node should return end

  it = tree.find(30);
  ++it;
  EXPECT_EQ(it, tree.end());  // Moving past the last node should return end
}

TEST_F(SetTest, GetMaxNode) {
  s21::RBTree<int> tree;

  tree.insert(10);
  tree.insert(20);
  tree.insert(5);

  auto it = tree.begin();  // Start at minimum node
  EXPECT_EQ(*it, 5);

  auto maxNode = it.getMaxNode();
  EXPECT_EQ(maxNode->value_, 20);  // Ensure maximum node has the correct value
}

TEST_F(SetTest, DereferenceIterator) {
  s21::RBTree<int> tree;
  tree.insert(10);
  tree.insert(20);

  auto it = tree.begin();

  // Dereference using operator*
  EXPECT_EQ(*it, 10);

  // Dereference using operator-> (should return a pointer)
  EXPECT_EQ(*(it.operator->()), 10);

  // Move to the next node and test again
  ++it;
  EXPECT_EQ(*it, 20);
  EXPECT_EQ(*(it.operator->()), 20);
}

TEST_F(SetTest, DereferenceEndIterator) {
  s21::RBTree<int> tree;

  // Insert elements
  tree.insert(10);
  tree.insert(20);

  auto it = tree.end();

  // Dereferencing end iterator is undefined, but we check for no exception
  EXPECT_NO_THROW(*it);
  EXPECT_NO_THROW(it.operator->());
}

TEST_F(SetTest, DereferenceNullIterator) {
  s21::RBTree<int> tree;

  auto it = tree.begin();  // Should point to the sentinel node in an empty tree

  // Dereferencing an empty tree's iterator should not throw
  EXPECT_NO_THROW(*it);
  EXPECT_NO_THROW(it.operator->());
}

TEST_F(SetTest, DereferenceWithGetCurrentValue) {
  s21::RBTree<int> tree;
  tree.insert(15);

  auto it = tree.begin();

  // Use getCurrentValue explicitly
  EXPECT_EQ(it.getCurrentValue(), 15);

  // Ensure it works consistently with operator*
  EXPECT_EQ(*it, 15);
}

TEST_F(SetTest, ReverseIteration) {
  default_set.insert(1);
  default_set.insert(2);
  default_set.insert(3);

  auto it = default_set.end();
  --it;
  EXPECT_EQ(*it, 3);

  --it;
  EXPECT_EQ(*it, 2);

  --it;
  EXPECT_EQ(*it, 1);
}

TEST_F(SetTest, EmptySetSizeAndIterators) {
  EXPECT_EQ(default_set.size(), 0);
  EXPECT_TRUE(default_set.empty());
  EXPECT_EQ(default_set.begin(), default_set.end());
}

TEST_F(SetTest, UpperLowerBound) {
  default_set.insert(10);
  default_set.insert(20);
  default_set.insert(30);

  auto lb = default_set.lower_bound(15);
  EXPECT_EQ(*lb, 20);  // First element not less than 15

  auto ub = default_set.upper_bound(20);
  EXPECT_EQ(*ub, 30);  // First element greater than 20
}

TEST_F(SetTest, IteratorBoundaryBehavior) {
  default_set.insert(10);
  default_set.insert(20);

  auto it = default_set.begin();
  EXPECT_EQ(*it, 10);

  --it;  // Should stay at end_node_
  EXPECT_EQ(it, default_set.end());
}

TEST_F(SetTest, RangeBasedForLoop) {
  default_set.insert(1);
  default_set.insert(2);
  default_set.insert(3);

  int sum = 0;
  for (const auto& val : default_set) {
    sum += val;
  }
  EXPECT_EQ(sum, 6);
}

TEST_F(SetTest, ClearAndReuse) {
  default_set.insert(1);
  default_set.insert(2);
  default_set.clear();

  EXPECT_EQ(default_set.size(), 0);
  EXPECT_TRUE(default_set.empty());

  default_set.insert(3);
  EXPECT_EQ(default_set.size(), 1);
  EXPECT_EQ(*default_set.begin(), 3);
}

TEST_F(SetTest, IteratorCopyAssignment) {
  default_set.insert(1);
  default_set.insert(2);

  auto it1 = default_set.begin();
  auto it2 = it1;  // Copy constructor

  EXPECT_EQ(*it1, *it2);

  ++it1;
  EXPECT_NE(*it1, *it2);

  it2 = it1;  // Assignment operator
  EXPECT_EQ(*it1, *it2);
}

TEST_F(SetTest, StressTestReverseIteration) {
  for (int i = 1; i <= 1000; ++i) {
    default_set.insert(i);
  }

  int value = 1000;
  for (auto it = default_set.end(); it != default_set.begin();) {
    --it;
    EXPECT_EQ(*it, value--);
  }
  EXPECT_EQ(value, 0);
}

TEST_F(SetTest, IteratorMoveSemantics) {
  default_set.insert(1);
  default_set.insert(2);

  auto it1 = default_set.begin();
  auto it2 = std::move(it1);

  EXPECT_EQ(*it2, 1);                 // Ownership transferred
  EXPECT_EQ(it1, default_set.end());  // it1 is in a valid but unspecified state
}

TEST_F(SetTest, SwapWithEmptySet) {
  Set<int> empty_set;
  default_set.insert(1);
  default_set.insert(2);

  default_set.swap(empty_set);

  EXPECT_TRUE(default_set.empty());
  EXPECT_EQ(empty_set.size(), 2);
  EXPECT_NE(empty_set.find(1), empty_set.end());
  EXPECT_NE(empty_set.find(2), empty_set.end());
}

TEST_F(SetTest, SwapWithCustomComparatorSet) {
  Set<int, std::greater<int>> another_custom_set{std::greater<int>()};
  another_custom_set.insert(1);
  another_custom_set.insert(2);

  custom_comp_set.swap(another_custom_set);

  EXPECT_TRUE(another_custom_set.empty());
  EXPECT_EQ(custom_comp_set.size(), 2);
  EXPECT_NE(custom_comp_set.find(1), custom_comp_set.end());
  EXPECT_NE(custom_comp_set.find(2), custom_comp_set.end());
}

TEST_F(SetTest, MaxSize) {
  EXPECT_GT(default_set.max_size(), 0);
  EXPECT_GT(custom_comp_set.max_size(), 0);
}

TEST_F(SetTest, EqualRange) {
  default_set.insert(10);
  default_set.insert(20);
  default_set.insert(30);

  auto range = default_set.equal_range(20);
  EXPECT_EQ(*range.first, 20);
  EXPECT_EQ(range.second, default_set.upper_bound(20));
}

TEST_F(SetTest, LowerBoundAndUpperBound) {
  default_set.insert(5);
  default_set.insert(10);
  default_set.insert(15);

  auto lower = default_set.lower_bound(10);
  auto upper = default_set.upper_bound(10);

  ASSERT_NE(lower, default_set.end());
  EXPECT_EQ(*lower, 10);

  ASSERT_NE(upper, default_set.end());
  EXPECT_EQ(*upper, 15);
}

TEST_F(SetTest, Count) {
  default_set.insert(42);
  EXPECT_EQ(default_set.count(42), 1);
  EXPECT_EQ(default_set.count(7), 0);
}

TEST_F(SetTest, EraseByIterator) {
  default_set.insert(42);
  auto it = default_set.find(42);
  ASSERT_NE(it, default_set.end());

  default_set.erase(it);

  EXPECT_EQ(default_set.find(42), default_set.end());
  EXPECT_TRUE(default_set.empty());
}

TEST_F(SetTest, SelfAssignment) {
  default_set.insert(1);
  default_set.insert(2);

  default_set = default_set;

  EXPECT_EQ(default_set.size(), 2);
  EXPECT_NE(default_set.find(1), default_set.end());
  EXPECT_NE(default_set.find(2), default_set.end());
}

TEST_F(SetTest, ClearAndInsert) {
  default_set.insert(42);
  default_set.insert(84);

  default_set.clear();
  EXPECT_TRUE(default_set.empty());

  default_set.insert(21);
  EXPECT_EQ(default_set.size(), 1);
  EXPECT_NE(default_set.find(21), default_set.end());
}

TEST_F(SetTest, MergeWithDisjointSets) {
  Set<int> another_set;
  another_set.insert(5);
  another_set.insert(15);

  default_set.insert(10);
  default_set.merge(another_set);

  EXPECT_EQ(default_set.size(), 3);
  EXPECT_TRUE(another_set.empty());
  EXPECT_NE(default_set.find(5), default_set.end());
  EXPECT_NE(default_set.find(10), default_set.end());
  EXPECT_NE(default_set.find(15), default_set.end());
}

TEST_F(SetTest, MergeWithOverlappingSets) {
  Set<int> another_set;
  another_set.insert(5);
  another_set.insert(10);

  default_set.insert(10);
  default_set.insert(15);

  default_set.merge(another_set);

  EXPECT_EQ(default_set.size(), 3);  // No duplicate "10" should exist
  EXPECT_TRUE(another_set.empty());
  EXPECT_NE(default_set.find(5), default_set.end());
  EXPECT_NE(default_set.find(10), default_set.end());
  EXPECT_NE(default_set.find(15), default_set.end());
}

TEST_F(SetTest, MergeWithEmptySet) {
  Set<int> empty_set;

  default_set.insert(1);
  default_set.merge(empty_set);

  EXPECT_EQ(default_set.size(), 1);
  EXPECT_EQ(*default_set.begin(), 1);
  EXPECT_TRUE(empty_set.empty());
}

TEST_F(SetTest, MergeEmptyIntoNonEmpty) {
  default_set.insert(1);
  default_set.insert(2);

  Set<int> empty_set;
  empty_set.merge(default_set);

  EXPECT_TRUE(default_set.empty());
  EXPECT_EQ(empty_set.size(), 2);
  EXPECT_NE(empty_set.find(1), empty_set.end());
  EXPECT_NE(empty_set.find(2), empty_set.end());
}

TEST_F(SetTest, FindNonExistentKey) {
  default_set.insert(10);
  default_set.insert(20);

  EXPECT_EQ(default_set.find(30), default_set.end());
}

TEST_F(SetTest, FindOnEmptySet) {
  EXPECT_EQ(default_set.find(42), default_set.end());
}

TEST_F(SetTest, FindWithCustomComparator) {
  custom_comp_set.insert(5);
  custom_comp_set.insert(15);

  auto it = custom_comp_set.find(5);
  ASSERT_NE(it, custom_comp_set.end());
  EXPECT_EQ(*it, 5);
}

TEST_F(SetTest, ConstIteratorDereference) {
  default_set.insert(42);

  const auto& const_set = default_set;
  auto it = const_set.find(42);

  ASSERT_NE(it, const_set.end());
  EXPECT_EQ(*it, 42);
}

TEST_F(SetTest, ConstIteratorBoundaryBehavior) {
  default_set.insert(10);
  default_set.insert(20);

  const auto& const_set = default_set;
  auto it = const_set.end();

  EXPECT_NO_THROW(*it);
}

TEST_F(SetTest, LargeInsertionStressTest) {
  constexpr int large_size = 10000;
  for (int i = 0; i < large_size; ++i) {
    default_set.insert(i);
  }

  EXPECT_EQ(default_set.size(), large_size);
  EXPECT_NE(default_set.find(large_size - 1), default_set.end());
}

TEST_F(SetTest, ExceptionSafetyDuringInsertion) {
  class ThrowingComparator {
   public:
    bool operator()(int a, int b) const {
      if (a == 42 || b == 42) throw std::runtime_error("Test exception");
      return a < b;
    }
  };

  Set<int, ThrowingComparator> throwing_set;
  EXPECT_NO_THROW(throwing_set.insert(42));
  EXPECT_FALSE(throwing_set.empty());
}

}  // namespace s21
