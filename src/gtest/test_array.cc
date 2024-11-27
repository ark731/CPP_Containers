// test_array.cpp
#include <gtest/gtest.h>

// Include necessary headers
#include <algorithm>
#include <iterator>

#include "../s21_containersplus.h"

namespace s21 {
class ArrayTest : public ::testing::Test {};

// Test default constructor
TEST_F(ArrayTest, DefaultConstructor) {
  Array<int, 5> arr;
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr[i], int{})
        << "Element at index " << i << " should be default-initialized";
  }
}

// Test initializer list constructor
TEST_F(ArrayTest, InitializerListConstructor) {
  Array<int, 5> arr = {1, 2, 3};
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 3);
  EXPECT_EQ(arr[3], int{});
  EXPECT_EQ(arr[4], int{});
}

// Test copy constructor
TEST_F(ArrayTest, CopyConstructor) {
  Array<int, 5> original = {1, 2, 3, 4, 5};
  Array<int, 5> copy(original);
  for (size_t i = 0; i < copy.size(); ++i) {
    EXPECT_EQ(copy[i], original[i]);
  }
}

// Test move constructor
TEST_F(ArrayTest, MoveConstructor) {
  Array<int, 5> original = {1, 2, 3, 4, 5};
  Array<int, 5> moved(std::move(original));
  EXPECT_EQ(moved[0], 1);
  EXPECT_EQ(moved[1], 2);
  EXPECT_EQ(moved[2], 3);
  EXPECT_EQ(moved[3], 4);
  EXPECT_EQ(moved[4], 5);
}

// Test copy assignment operator
TEST_F(ArrayTest, CopyAssignmentOperator) {
  Array<int, 5> original = {5, 4, 3, 2, 1};
  Array<int, 5> assigned;
  assigned = original;
  for (size_t i = 0; i < assigned.size(); ++i) {
    EXPECT_EQ(assigned[i], original[i]);
  }
}

// Test move assignment operator
TEST_F(ArrayTest, MoveAssignmentOperator) {
  Array<int, 5> original = {5, 4, 3, 2, 1};
  Array<int, 5> assigned;
  assigned = std::move(original);
  EXPECT_EQ(assigned[0], 5);
  EXPECT_EQ(assigned[1], 4);
  EXPECT_EQ(assigned[2], 3);
  EXPECT_EQ(assigned[3], 2);
  EXPECT_EQ(assigned[4], 1);
}

// Test operator[]
TEST_F(ArrayTest, OperatorSquareBrackets) {
  Array<int, 5> arr = {10, 20, 30, 40, 50};
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[1], 20);
  EXPECT_EQ(arr[2], 30);
  EXPECT_EQ(arr[3], 40);
  EXPECT_EQ(arr[4], 50);
}

// Test at() method with valid index
TEST_F(ArrayTest, AtMethodValidIndex) {
  Array<int, 5> arr = {100, 200, 300, 400, 500};
  EXPECT_EQ(arr.at(0), 100);
  EXPECT_EQ(arr.at(4), 500);
}

// Test at() method with invalid index
TEST_F(ArrayTest, AtMethodInvalidIndex) {
  Array<int, 5> arr;
  EXPECT_THROW(arr.at(5), std::out_of_range);
  EXPECT_THROW(arr.at(10), std::out_of_range);
}

// Test front() method
TEST_F(ArrayTest, FrontMethod) {
  Array<int, 5> arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(arr.front(), 1);
}

// Test back() method
TEST_F(ArrayTest, BackMethod) {
  Array<int, 5> arr = {1, 2, 3, 4, 5};
  EXPECT_EQ(arr.back(), 5);
}

// Test empty() method
TEST_F(ArrayTest, EmptyMethod) {
  Array<int, 0> arr_empty;
  EXPECT_TRUE(arr_empty.empty());

  Array<int, 5> arr_non_empty;
  EXPECT_FALSE(arr_non_empty.empty());
}

// Test size() method
TEST_F(ArrayTest, SizeMethod) {
  Array<int, 0> arr_empty;
  EXPECT_EQ(arr_empty.size(), 0);

  Array<int, 5> arr;
  EXPECT_EQ(arr.size(), 5);
}

// Test max_size() method
TEST_F(ArrayTest, MaxSizeMethod) {
  Array<int, 5> arr;
  EXPECT_EQ(arr.max_size(), 5);
}

// Test fill() method
TEST_F(ArrayTest, FillMethod) {
  Array<int, 5> arr;
  arr.fill(42);
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr[i], 42);
  }
}

// Test swap() method
TEST_F(ArrayTest, SwapMethod) {
  Array<int, 5> arr1 = {1, 2, 3, 4, 5};
  Array<int, 5> arr2 = {5, 4, 3, 2, 1};
  arr1.swap(arr2);
  EXPECT_EQ(arr1[0], 5);
  EXPECT_EQ(arr1[1], 4);
  EXPECT_EQ(arr1[2], 3);
  EXPECT_EQ(arr1[3], 2);
  EXPECT_EQ(arr1[4], 1);
  EXPECT_EQ(arr2[0], 1);
  EXPECT_EQ(arr2[1], 2);
  EXPECT_EQ(arr2[2], 3);
  EXPECT_EQ(arr2[3], 4);
  EXPECT_EQ(arr2[4], 5);
}

// Test iterator begin() and end()
TEST_F(ArrayTest, IteratorBeginEnd) {
  Array<int, 5> arr = {10, 20, 30, 40, 50};
  int expected_value = 10;
  for (auto it = arr.begin(); it != arr.end(); ++it) {
    EXPECT_EQ(*it, expected_value);
    expected_value += 10;
  }
}

// Test const_iterator begin() and end()
TEST_F(ArrayTest, ConstIteratorBeginEnd) {
  const Array<int, 5> arr = {10, 20, 30, 40, 50};
  int expected_value = 10;
  for (auto it = arr.begin(); it != arr.end(); ++it) {
    EXPECT_EQ(*it, expected_value);
    expected_value += 10;
  }
}

// Test iterator increment and decrement
TEST_F(ArrayTest, IteratorIncrementDecrement) {
  Array<int, 5> arr = {1, 2, 3, 4, 5};
  auto it = arr.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  it++;
  EXPECT_EQ(*it, 3);
  --it;
  EXPECT_EQ(*it, 2);
  it--;
  EXPECT_EQ(*it, 1);
}

// Test iterator equality and inequality
TEST_F(ArrayTest, IteratorEquality) {
  Array<int, 5> arr = {1, 2, 3, 4, 5};
  auto it1 = arr.begin();
  auto it2 = arr.begin();
  EXPECT_TRUE(it1 == it2);
  ++it2;
  EXPECT_TRUE(it1 != it2);
}

// Test modifying elements through iterator
TEST_F(ArrayTest, ModifyThroughIterator) {
  Array<int, 5> arr = {1, 2, 3, 4, 5};
  for (auto it = arr.begin(); it != arr.end(); ++it) {
    *it *= 2;
  }
  EXPECT_EQ(arr[0], 2);
  EXPECT_EQ(arr[1], 4);
  EXPECT_EQ(arr[2], 6);
  EXPECT_EQ(arr[3], 8);
  EXPECT_EQ(arr[4], 10);
}

// Test const_iterator cannot modify elements
TEST_F(ArrayTest, ConstIteratorCannotModify) {
  Array<int, 5> arr = {1, 2, 3, 4, 5};
  const auto& const_arr = arr;
  auto it = const_arr.begin();
  EXPECT_EQ(*it, 1);
}

// Test reverse iteration
TEST_F(ArrayTest, ReverseIteration) {
  Array<int, 5> arr = {10, 20, 30, 40, 50};
  int expected_value = 50;
  for (auto it = arr.end(); it != arr.begin();) {
    --it;
    EXPECT_EQ(*it, expected_value);
    expected_value -= 10;
  }
}

// Test that default-initialized elements are zero-initialized
TEST_F(ArrayTest, DefaultInitialization) {
  Array<int, 5> arr;
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr[i], 0);
  }
}

// Test array of objects with constructors
TEST_F(ArrayTest, ArrayOfObjects) {
  struct TestStruct {
    int x;
    TestStruct() : x(0) {}
    TestStruct(int val) : x(val) {}
  };
  Array<TestStruct, 3> arr = {TestStruct(1), TestStruct(2), TestStruct(3)};
  EXPECT_EQ(arr[0].x, 1);
  EXPECT_EQ(arr[1].x, 2);
  EXPECT_EQ(arr[2].x, 3);
}

// Test assignment to elements
TEST_F(ArrayTest, AssignmentToElements) {
  Array<int, 5> arr;
  arr[0] = 100;
  arr.at(1) = 200;
  arr.front() = 300;
  arr.back() = 400;
  EXPECT_EQ(arr[0], 300);
  EXPECT_EQ(arr[1], 200);
  EXPECT_EQ(arr[4], 400);
}

// Test modifying array through const reference should not compile
TEST_F(ArrayTest, ConstArrayModification) {
  Array<int, 5> arr = {1, 2, 3, 4, 5};
  const Array<int, 5>& const_arr = arr;
  EXPECT_EQ(const_arr[0], 1);
}

// Test array with size zero
TEST_F(ArrayTest, ZeroSizedArray) {
  Array<int, 0> arr;
  EXPECT_EQ(arr.size(), 0);
  EXPECT_TRUE(arr.empty());
  EXPECT_TRUE(arr.begin() == arr.end());
}

// Test using iterators in standard algorithms
TEST_F(ArrayTest, IteratorsInAlgorithms) {
  Array<int, 5> arr = {5, 4, 3, 2, 1};
  std::sort(arr.begin(), arr.end());
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr[i], static_cast<int>(i + 1));
  }
}

// Test iterator arithmetic
TEST_F(ArrayTest, IteratorArithmetic) {
  Array<int, 5> arr = {10, 20, 30, 40, 50};
  auto it = arr.begin();
  it += 2;
  EXPECT_EQ(*it, 30);
  it = it + 2;
  EXPECT_EQ(*it, 50);
  it = it - 3;
  EXPECT_EQ(*it, 20);
  it -= 1;
  EXPECT_EQ(*it, 10);
}

// Test iterator difference
TEST_F(ArrayTest, IteratorDifference) {
  Array<int, 5> arr;
  auto it_begin = arr.begin();
  auto it_end = arr.end();
  EXPECT_EQ(it_end - it_begin, arr.size());
}
// Test iterator relational operators
TEST_F(ArrayTest, IteratorRelationalOperators) {
  Array<int, 5> arr;
  auto it1 = arr.begin();
  auto it2 = arr.begin() + 2;
  EXPECT_TRUE(it1 < it2);
  EXPECT_TRUE(it1 <= it2);
  EXPECT_FALSE(it1 > it2);
  EXPECT_FALSE(it1 >= it2);
}

// Test iterator arithmetic operations
TEST_F(ArrayTest, IteratorArithmeticOperations) {
  Array<int, 5> arr = {1, 2, 3, 4, 5};
  auto it = arr.begin();

  // Test addition
  auto it_plus_2 = it + 2;
  EXPECT_EQ(*it_plus_2, 3);

  // Test subtraction
  auto it_minus_1 = it_plus_2 - 1;
  EXPECT_EQ(*it_minus_1, 2);

  // Test compound assignment addition
  it += 3;
  EXPECT_EQ(*it, 4);

  // Test compound assignment subtraction
  it -= 2;
  EXPECT_EQ(*it, 2);
}

// Test iterator relational operators
TEST_F(ArrayTest, IteratorRelationalOperators2) {
  Array<int, 5> arr = {10, 20, 30, 40, 50};
  auto it_begin = arr.begin();
  auto it_mid = arr.begin() + 2;
  auto it_end = arr.end();

  EXPECT_TRUE(it_begin < it_mid);
  EXPECT_TRUE(it_mid < it_end);
  EXPECT_TRUE(it_begin <= it_mid);
  EXPECT_TRUE(it_mid <= it_end);
  EXPECT_FALSE(it_end < it_begin);
  EXPECT_TRUE(it_end > it_mid);
  EXPECT_TRUE(it_end >= it_mid);
}

// Test iterator difference
TEST_F(ArrayTest, IteratorDifference2) {
  Array<int, 5> arr;
  auto it_begin = arr.begin();
  auto it_end = arr.end();
  EXPECT_EQ(it_end - it_begin, 5);

  auto it_mid = arr.begin() + 2;
  EXPECT_EQ(it_mid - it_begin, 2);
  EXPECT_EQ(it_end - it_mid, 3);
}

// Test iterator indexing operator
TEST_F(ArrayTest, IteratorIndexingOperator) {
  Array<int, 5> arr = {5, 4, 3, 2, 1};
  auto it = arr.begin();
  EXPECT_EQ(it[0], 5);
  EXPECT_EQ(it[2], 3);
  EXPECT_EQ(it[4], 1);
}

// Test const iterator arithmetic operations
TEST_F(ArrayTest, ConstIteratorArithmeticOperations) {
  const Array<int, 5> arr = {2, 4, 6, 8, 10};
  auto it = arr.cbegin();

  // Test addition
  auto it_plus_3 = it + 3;
  EXPECT_EQ(*it_plus_3, 8);

  // Test subtraction
  auto it_minus_1 = it_plus_3 - 1;
  EXPECT_EQ(*it_minus_1, 6);

  // Test compound assignment addition
  it += 2;
  EXPECT_EQ(*it, 6);

  // Test compound assignment subtraction
  it -= 1;
  EXPECT_EQ(*it, 4);
}

// Test reverse iteration using random access
TEST_F(ArrayTest, ReverseIterationRandomAccess) {
  Array<int, 5> arr = {10, 20, 30, 40, 50};
  auto it = arr.end() - 1;
  int expected_value = 50;
  for (; it >= arr.begin(); --it) {
    EXPECT_EQ(*it, expected_value);
    expected_value -= 10;
  }
}

// Test modifying elements using iterator arithmetic
TEST_F(ArrayTest, ModifyElementsUsingIteratorArithmetic) {
  Array<int, 5> arr = {1, 2, 3, 4, 5};
  auto it = arr.begin();
  *(it + 2) = 10;  // Modify the third element
  EXPECT_EQ(arr[2], 10);

  *(it += 1) = 20;  // Modify the second element
  EXPECT_EQ(arr[1], 20);

  *(it -= 1) = 30;  // Modify the first element
  EXPECT_EQ(arr[0], 30);
}

// Test using iterator in standard algorithms that require random access
TEST_F(ArrayTest, IteratorInRandomAccessAlgorithms) {
  Array<int, 5> arr = {5, 3, 4, 1, 2};

  // Use std::nth_element, which requires random access iterators
  std::nth_element(arr.begin(), arr.begin() + 2, arr.end());

  // The third smallest element should now be at position 2
  EXPECT_EQ(arr[2], 3);

  // Check that the array is partitioned correctly
  EXPECT_LE(arr[0], arr[2]);
  EXPECT_LE(arr[1], arr[2]);
  EXPECT_GE(arr[3], arr[2]);
  EXPECT_GE(arr[4], arr[2]);
}

// Test distance between iterators
TEST_F(ArrayTest, IteratorDistance) {
  Array<int, 5> arr;
  auto it_begin = arr.begin();
  auto it_end = arr.end();
  EXPECT_EQ(std::distance(it_begin, it_end), 5);
}

// Test accessing elements in reverse using negative indices
TEST_F(ArrayTest, AccessElementsReverseIndexing) {
  Array<int, 5> arr = {10, 20, 30, 40, 50};
  auto it = arr.end() - 1;
  EXPECT_EQ(it[0], 50);
  EXPECT_EQ(it[-1], 40);
  EXPECT_EQ(it[-4], 10);
}

// Test pointer difference with const iterators
TEST_F(ArrayTest, ConstIteratorDifference) {
  const Array<int, 5> arr;
  auto it_begin = arr.cbegin();
  auto it_end = arr.cend();
  EXPECT_EQ(it_end - it_begin, 5);
}

// Test using reverse iterators (if implemented)
TEST_F(ArrayTest, ReverseIterators) {
  Array<int, 5> arr = {1, 2, 3, 4, 5};
  std::reverse_iterator<Array<int, 5>::Iterator> rbegin(arr.end());
  std::reverse_iterator<Array<int, 5>::Iterator> rend(arr.begin());

  int expected = 5;
  for (auto it = rbegin; it != rend; ++it) {
    EXPECT_EQ(*it, expected);
    --expected;
  }
}

// Test iterator's subscript operator with negative indices
TEST_F(ArrayTest, IteratorSubscriptNegativeIndex) {
  Array<int, 5> arr = {10, 20, 30, 40, 50};
  auto it = arr.begin() + 4;
  EXPECT_EQ(it[0], 50);
  EXPECT_EQ(it[-1], 40);
  EXPECT_EQ(it[-4], 10);
}

// Test that iterator arithmetic maintains valid ranges
TEST_F(ArrayTest, IteratorArithmeticValidRange) {
  Array<int, 5> arr;
  auto it_begin = arr.begin();
  auto it_end = arr.end();

  EXPECT_NO_THROW({
    auto it = it_begin + 5;
    EXPECT_TRUE(it == it_end);
  });

  EXPECT_NO_THROW({
    auto it = it_end - 5;
    EXPECT_TRUE(it == it_begin);
  });
}

// Test using iterators with algorithms that require random access
TEST_F(ArrayTest, IteratorWithRandomAccessAlgorithms) {
  Array<int, 5> arr = {3, 1, 4, 5, 2};
  std::partial_sort(arr.begin(), arr.begin() + 3, arr.end());
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 3);
}

// Test iterator arithmetic beyond array bounds (should be avoided)
TEST_F(ArrayTest, IteratorArithmeticBeyondBounds) {
  Array<int, 5> arr;
  auto it = arr.begin();

  // Incrementing iterator beyond end
  it = it + 10;  // Undefined behavior, but we can test that it doesn't crash
  // Do not dereference 'it' here as it is out of bounds
}

// Test using standard algorithms like std::reverse
TEST_F(ArrayTest, UseStdReverse) {
  Array<int, 5> arr = {1, 2, 3, 4, 5};
  std::reverse(arr.begin(), arr.end());
  EXPECT_EQ(arr[0], 5);
  EXPECT_EQ(arr[1], 4);
  EXPECT_EQ(arr[2], 3);
  EXPECT_EQ(arr[3], 2);
  EXPECT_EQ(arr[4], 1);
}

// Test const correctness with iterator arithmetic
TEST_F(ArrayTest, ConstCorrectnessIteratorArithmetic) {
  const Array<int, 5> arr = {1, 2, 3, 4, 5};
  auto it = arr.cbegin();
  EXPECT_EQ(*(it + 2), 3);
  EXPECT_EQ(*(it), 1);
}

// Test that iterator behaves correctly at the bounds
TEST_F(ArrayTest, IteratorAtBounds) {
  Array<int, 5> arr = {10, 20, 30, 40, 50};
  auto it_begin = arr.begin();
  auto it_end = arr.end();

  EXPECT_EQ(*it_begin, 10);
  EXPECT_EQ(*(it_end - 1), 50);
  EXPECT_TRUE(it_begin < it_end);
  EXPECT_TRUE(it_begin <= it_end - 1);
  EXPECT_FALSE(it_end <= it_begin);
}

}  // namespace s21