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
  std::cout << default_set.size();
  std::cout << std::endl;
  auto results = default_set.insert_many(1, 2, 3, 4);
  EXPECT_EQ(default_set.size(), 4);
  std::cout << default_set.size();
  std::cout << std::endl;
  for (int i = 1; i <= 4; ++i) {
    EXPECT_TRUE(default_set.count(i));
    std::cout << default_set.count(i);
    std::cout << std::endl;
    default_set.printTree();
  }
}

// // Test erase method
// TEST_F(SetTest, Erase) {
//   default_set.insert(10);
//   default_set.insert(20);

//   auto it = default_set.find(10);
//   default_set.erase(it);

//   EXPECT_EQ(default_set.size(), 1);
//   EXPECT_FALSE(default_set.count(10));
//   EXPECT_TRUE(default_set.count(20));

//   // Erase non-existent element
//   size_t erase_result = default_set.erase(30);
//   EXPECT_EQ(erase_result, 0);  // No elements were erased
// }

// // Test clear method
// TEST_F(SetTest, Clear) {
//   default_set.insert(10);
//   default_set.insert(20);

//   default_set.clear();

//   EXPECT_TRUE(default_set.empty());
//   EXPECT_EQ(default_set.size(), 0);
// }

// // Test custom comparator: std::greater<int>
// TEST_F(SetTest, CustomComparatorOrder) {
//   custom_comp_set.insert(10);
//   custom_comp_set.insert(20);
//   custom_comp_set.insert(30);

//   // The expected order should be 30, 20, 10 for std::greater<int>
//   auto it = custom_comp_set.begin();
//   EXPECT_EQ(*it, 30);

//   it = std::next(it);
//   EXPECT_EQ(*it, 20);

//   it = std::next(it);
//   EXPECT_EQ(*it, 10);
// }

// // Test iterators: begin and end
// TEST_F(SetTest, Iterators) {
//   default_set.insert(10);
//   default_set.insert(20);
//   default_set.insert(30);

//   auto it = default_set.begin();
//   EXPECT_EQ(*it, 10);  // Should be the smallest element

//   it = std::next(it);
//   EXPECT_EQ(*it, 20);

//   it = std::next(it);
//   EXPECT_EQ(*it, 30);

//   // Make sure next is handled correctly at the end of the container
//   it = std::next(it);
//   EXPECT_EQ(it, default_set.end());  // After last element, next should be
//   end()
// }

}  // namespace s21
