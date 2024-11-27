#include <gtest/gtest.h>

#include <utility>

#include "../s21_containers.h"

namespace s21 {
class VectorTest : public ::testing::Test {};

// Test default constructor
TEST_F(VectorTest, DefaultConstructor) {
  Vector<int> vec;
  EXPECT_TRUE(vec.empty());
  EXPECT_EQ(vec.size(), 0);
  EXPECT_EQ(vec.capacity(), 0);
}

// Test constructor with initial size
TEST_F(VectorTest, ConstructorWithSize) {
  Vector<int> vec(5);
  EXPECT_FALSE(vec.empty());
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec.capacity(), 5);
  for (size_t i = 0; i < vec.size(); ++i) {
    EXPECT_EQ(vec[i], int{});
  }
}

// Test initializer list constructor
TEST_F(VectorTest, InitializerListConstructor) {
  Vector<int> vec = {1, 2, 3};
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec.capacity(), 3);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
}

// Test copy constructor
TEST_F(VectorTest, CopyConstructor) {
  Vector<int> original = {1, 2, 3};
  Vector<int> copy(original);
  EXPECT_EQ(copy.size(), 3);
  EXPECT_EQ(copy.capacity(), 3);
  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
  EXPECT_EQ(copy[2], 3);
}

// Test move constructor
TEST_F(VectorTest, MoveConstructor) {
  Vector<int> original = {1, 2, 3};
  Vector<int> moved(std::move(original));
  EXPECT_EQ(moved.size(), 3);
  EXPECT_EQ(moved.capacity(), 3);
  EXPECT_EQ(moved[0], 1);
  EXPECT_EQ(moved[1], 2);
  EXPECT_EQ(moved[2], 3);
  EXPECT_EQ(original.size(), 0);
  EXPECT_EQ(original.capacity(), 0);
}

// Test copy assignment operator
TEST_F(VectorTest, CopyAssignmentOperator) {
  Vector<int> original = {1, 2, 3};
  Vector<int> copy;
  copy = original;
  EXPECT_EQ(copy.size(), 3);
  EXPECT_EQ(copy[0], 1);
  EXPECT_EQ(copy[1], 2);
  EXPECT_EQ(copy[2], 3);
}

// Test move assignment operator
TEST_F(VectorTest, MoveAssignmentOperator) {
  Vector<int> original = {1, 2, 3};
  Vector<int> moved;
  moved = std::move(original);
  EXPECT_EQ(moved.size(), 3);
  EXPECT_EQ(moved[0], 1);
  EXPECT_EQ(moved[1], 2);
  EXPECT_EQ(moved[2], 3);
  EXPECT_EQ(original.size(), 0);
  EXPECT_EQ(original.capacity(), 0);
}

// Test operator[]
TEST_F(VectorTest, OperatorSquareBrackets) {
  Vector<int> vec = {10, 20, 30};
  EXPECT_EQ(vec[0], 10);
  EXPECT_EQ(vec[1], 20);
  EXPECT_EQ(vec[2], 30);
  vec[1] = 25;
  EXPECT_EQ(vec[1], 25);
}

// Test at() method with valid index
TEST_F(VectorTest, AtMethodValidIndex) {
  Vector<int> vec = {100, 200, 300};
  EXPECT_EQ(vec.at(0), 100);
  EXPECT_EQ(vec.at(2), 300);
}

// Test at() method with invalid index
TEST_F(VectorTest, AtMethodInvalidIndex) {
  Vector<int> vec(3);
  EXPECT_THROW(vec.at(3), std::out_of_range);
  EXPECT_THROW(vec.at(10), std::out_of_range);
}

// Test front() method
TEST_F(VectorTest, FrontMethod) {
  Vector<int> vec = {1, 2, 3};
  EXPECT_EQ(vec.front(), 1);
}

// Test back() method
TEST_F(VectorTest, BackMethod) {
  Vector<int> vec = {1, 2, 3};
  EXPECT_EQ(vec.back(), 3);
}

// Test empty() method
TEST_F(VectorTest, EmptyMethod) {
  Vector<int> vec_empty;
  EXPECT_TRUE(vec_empty.empty());

  Vector<int> vec_non_empty = {1};
  EXPECT_FALSE(vec_non_empty.empty());
}

// Test size() method
TEST_F(VectorTest, SizeMethod) {
  Vector<int> vec_empty;
  EXPECT_EQ(vec_empty.size(), 0);

  Vector<int> vec(5);
  EXPECT_EQ(vec.size(), 5);
}

// Test capacity() method
TEST_F(VectorTest, CapacityMethod) {
  Vector<int> vec;
  EXPECT_EQ(vec.capacity(), 0);
  vec.reserve(10);
  EXPECT_EQ(vec.capacity(), 10);
}

// Test reserve() method
TEST_F(VectorTest, ReserveMethod) {
  Vector<int> vec;
  vec.reserve(5);
  EXPECT_EQ(vec.capacity(), 5);
  vec.reserve(10);
  EXPECT_EQ(vec.capacity(), 10);
  vec.reserve(5);  // Should not reduce capacity
  EXPECT_EQ(vec.capacity(), 10);
}

// Test shrink_to_fit() method
TEST_F(VectorTest, ShrinkToFitMethod) {
  Vector<int> vec;
  vec.reserve(10);
  EXPECT_EQ(vec.capacity(), 10);
  vec.shrink_to_fit();
  EXPECT_EQ(vec.capacity(), 0);

  vec.push_back(1);
  vec.push_back(2);
  vec.reserve(10);
  vec.shrink_to_fit();
  EXPECT_EQ(vec.capacity(), 2);
}

// Test clear() method
TEST_F(VectorTest, ClearMethod) {
  Vector<int> vec = {1, 2, 3};
  vec.clear();
  EXPECT_TRUE(vec.empty());
  EXPECT_EQ(vec.size(), 0);
}

// Test push_back() method
TEST_F(VectorTest, PushBackMethod) {
  Vector<int> vec;
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0], 10);
  EXPECT_EQ(vec[1], 20);
  EXPECT_EQ(vec[2], 30);
}

// Test pop_back() method
TEST_F(VectorTest, PopBackMethod) {
  Vector<int> vec = {1, 2, 3};
  vec.pop_back();
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  vec.pop_back();
  vec.pop_back();
  EXPECT_TRUE(vec.empty());
}

// Test insert() method
TEST_F(VectorTest, InsertMethod) {
  Vector<int> vec = {1, 2, 4, 5};
  auto it = vec.insert(vec.begin() + 2, 3);
  EXPECT_EQ(*it, 3);
  EXPECT_EQ(vec.size(), 5);
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(vec[i], i + 1);
  }
}

// Test erase() method
TEST_F(VectorTest, EraseMethod) {
  Vector<int> vec = {1, 2, 3, 4, 5};
  auto it = vec.erase(vec.begin() + 2);
  EXPECT_EQ(*it, 4);
  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 4);
  EXPECT_EQ(vec[3], 5);
}

// Test swap() method
TEST_F(VectorTest, SwapMethod) {
  Vector<int> vec1 = {1, 2, 3};
  Vector<int> vec2 = {4, 5, 6, 7};
  vec1.swap(vec2);
  EXPECT_EQ(vec1.size(), 4);
  EXPECT_EQ(vec2.size(), 3);
  EXPECT_EQ(vec1[0], 4);
  EXPECT_EQ(vec1[3], 7);
  EXPECT_EQ(vec2[0], 1);
  EXPECT_EQ(vec2[2], 3);
}

// Test insert_many() method
TEST_F(VectorTest, InsertManyMethod) {
  Vector<int> vec = {1, 2, 6, 7};
  auto it = vec.insert_many(vec.begin() + 2, 3, 4, 5);
  EXPECT_EQ(*it, 3);
  EXPECT_EQ(vec.size(), 7);
  for (int i = 0; i < 7; ++i) {
    EXPECT_EQ(vec[i], i + 1);
  }
}

// Test insert_many() at beginning
TEST_F(VectorTest, InsertManyAtBeginning) {
  Vector<int> vec = {4, 5, 6};
  vec.insert_many(vec.begin(), 1, 2, 3);
  EXPECT_EQ(vec.size(), 6);
  for (int i = 0; i < 6; ++i) {
    EXPECT_EQ(vec[i], i + 1);
  }
}

// Test insert_many() at end
TEST_F(VectorTest, InsertManyAtEnd) {
  Vector<int> vec = {1, 2, 3};
  vec.insert_many(vec.end(), 4, 5, 6);
  EXPECT_EQ(vec.size(), 6);
  for (int i = 0; i < 6; ++i) {
    EXPECT_EQ(vec[i], i + 1);
  }
}

// Test insert_many() into empty vector
TEST_F(VectorTest, InsertManyIntoEmptyVector) {
  Vector<int> vec;
  vec.insert_many(vec.begin(), 1, 2, 3);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
}

// Test iterator begin() and end()
TEST_F(VectorTest, IteratorBeginEnd) {
  Vector<int> vec = {10, 20, 30};
  int expected_value = 10;
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    EXPECT_EQ(*it, expected_value);
    expected_value += 10;
  }
}

// Test const_iterator begin() and end()
TEST_F(VectorTest, ConstIteratorBeginEnd) {
  const Vector<int> vec = {10, 20, 30};
  int expected_value = 10;
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    EXPECT_EQ(*it, expected_value);
    expected_value += 10;
  }
}

// Test iterator arithmetic operations
TEST_F(VectorTest, IteratorArithmeticOperations) {
  Vector<int> vec = {1, 2, 3, 4, 5};
  auto it = vec.begin();

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
TEST_F(VectorTest, IteratorRelationalOperators) {
  Vector<int> vec = {10, 20, 30, 40, 50};
  auto it_begin = vec.begin();
  auto it_mid = vec.begin() + 2;
  auto it_end = vec.end();

  EXPECT_TRUE(it_begin < it_mid);
  EXPECT_TRUE(it_mid < it_end);
  EXPECT_TRUE(it_begin <= it_mid);
  EXPECT_TRUE(it_mid <= it_end);
  EXPECT_FALSE(it_end < it_begin);
  EXPECT_TRUE(it_end > it_mid);
  EXPECT_TRUE(it_end >= it_mid);
}

// Test using iterator in standard algorithms
TEST_F(VectorTest, IteratorInAlgorithms) {
  Vector<int> vec = {5, 4, 3, 2, 1};
  std::sort(vec.begin(), vec.end());
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(vec[i], i + 1);
  }
}

// Test reverse iteration
TEST_F(VectorTest, ReverseIteration) {
  Vector<int> vec = {10, 20, 30, 40, 50};
  int expected_value = 50;
  for (auto it = vec.end() - 1; it >= vec.begin(); --it) {
    EXPECT_EQ(*it, expected_value);
    expected_value -= 10;
  }
}

// Test modifying elements through iterator
TEST_F(VectorTest, ModifyThroughIterator) {
  Vector<int> vec = {1, 2, 3, 4, 5};
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    *it *= 2;
  }
  EXPECT_EQ(vec[0], 2);
  EXPECT_EQ(vec[1], 4);
  EXPECT_EQ(vec[2], 6);
  EXPECT_EQ(vec[3], 8);
  EXPECT_EQ(vec[4], 10);
}

// Test const_iterator cannot modify elements
TEST_F(VectorTest, ConstIteratorCannotModify) {
  Vector<int> vec = {1, 2, 3, 4, 5};
  const auto& const_vec = vec;
  auto it = const_vec.begin();
  EXPECT_EQ(*it, 1);
}

// Test using reverse iterators (if implemented)
TEST_F(VectorTest, ReverseIterators) {
  Vector<int> vec = {1, 2, 3, 4, 5};
  std::reverse_iterator<Vector<int>::Iterator> rbegin(vec.end());
  std::reverse_iterator<Vector<int>::Iterator> rend(vec.begin());

  int expected = 5;
  for (auto it = rbegin; it != rend; ++it) {
    EXPECT_EQ(*it, expected);
    --expected;
  }
}

// Test exception safety in insert_many (using a type that can throw)
struct ExceptionThrowingType {
  int value;
  ExceptionThrowingType() : value(0) {}  // Default constructor
  ExceptionThrowingType(int v) : value(v) {}
  ExceptionThrowingType(const ExceptionThrowingType& other)
      : value(other.value) {
    if (value == -1) {
      throw std::runtime_error("Exception in copy constructor");
    }
  }
  ExceptionThrowingType& operator=(const ExceptionThrowingType& other) {
    value = other.value;
    if (value == -1) {
      throw std::runtime_error("Exception in assignment operator");
    }
    return *this;
  }
};

TEST_F(VectorTest, InsertManyExceptionSafety) {
  Vector<ExceptionThrowingType> vec = {1, 2, 3};
  EXPECT_THROW(
      vec.insert_many(vec.begin() + 1, ExceptionThrowingType(4),
                      ExceptionThrowingType(-1), ExceptionThrowingType(5)),
      std::runtime_error);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0].value, 1);
  EXPECT_EQ(vec[1].value, 2);
  EXPECT_EQ(vec[2].value, 3);
}

// Test data() method
TEST_F(VectorTest, DataMethod) {
  Vector<int> vec = {1, 2, 3};
  int* data_ptr = vec.data();
  EXPECT_EQ(data_ptr[0], 1);
  EXPECT_EQ(data_ptr[1], 2);
  EXPECT_EQ(data_ptr[2], 3);
}

// Test modifying elements through data()
TEST_F(VectorTest, ModifyThroughData) {
  Vector<int> vec(3);
  int* data_ptr = vec.data();
  data_ptr[0] = 10;
  data_ptr[1] = 20;
  data_ptr[2] = 30;
  EXPECT_EQ(vec[0], 10);
  EXPECT_EQ(vec[1], 20);
  EXPECT_EQ(vec[2], 30);
}

// Test assigning to elements
TEST_F(VectorTest, AssignmentToElements) {
  Vector<int> vec(3);
  vec[0] = 100;
  vec.at(1) = 200;
  vec.front() = 300;
  vec.back() = 400;
  EXPECT_EQ(vec[0], 300);
  EXPECT_EQ(vec[1], 200);
  EXPECT_EQ(vec[2], 400);
}

// Test using vector with custom objects
struct CustomObject {
  int x;
  CustomObject() : x(0) {}
  CustomObject(int val) : x(val) {}
  CustomObject(const CustomObject& other) : x(other.x) {}
  CustomObject& operator=(const CustomObject& other) {
    x = other.x;
    return *this;
  }
};

TEST_F(VectorTest, VectorOfCustomObjects) {
  Vector<CustomObject> vec = {CustomObject(1), CustomObject(2),
                              CustomObject(3)};
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0].x, 1);
  EXPECT_EQ(vec[1].x, 2);
  EXPECT_EQ(vec[2].x, 3);
}

// Test iterator's operator[]
TEST_F(VectorTest, IteratorIndexingOperator) {
  Vector<int> vec = {5, 4, 3, 2, 1};
  auto it = vec.begin();
  EXPECT_EQ(it[0], 5);
  EXPECT_EQ(it[2], 3);
  EXPECT_EQ(it[4], 1);
}

// Test const correctness with iterator arithmetic
TEST_F(VectorTest, ConstCorrectnessIteratorArithmetic) {
  const Vector<int> vec = {1, 2, 3, 4, 5};
  auto it = vec.cbegin();
  EXPECT_EQ(*(it + 2), 3);
  EXPECT_EQ(*(it), 1);
}

// Test iterator difference
TEST_F(VectorTest, IteratorDifference) {
  Vector<int> vec = {1, 2, 3, 4, 5};
  auto it_begin = vec.begin();
  auto it_end = vec.end();
  EXPECT_EQ(it_end - it_begin, 5);
}

// Test iterator's subscript operator with negative indices
TEST_F(VectorTest, IteratorSubscriptNegativeIndex) {
  Vector<int> vec = {10, 20, 30, 40, 50};
  auto it = vec.begin() + 4;
  EXPECT_EQ(it[0], 50);
  EXPECT_EQ(it[-1], 40);
  EXPECT_EQ(it[-4], 10);
}

// Test iterator arithmetic beyond vector bounds (should be avoided)
TEST_F(VectorTest, IteratorArithmeticBeyondBounds) {
  Vector<int> vec = {1, 2, 3};
  auto it = vec.begin();

  // Incrementing iterator beyond end
  it = it + 10;  // Undefined behavior, but we can test that it doesn't crash
  // Do not dereference 'it' here as it is out of bounds
}

// Test using standard algorithms like std::reverse
TEST_F(VectorTest, UseStdReverse) {
  Vector<int> vec = {1, 2, 3, 4, 5};
  std::reverse(vec.begin(), vec.end());
  EXPECT_EQ(vec[0], 5);
  EXPECT_EQ(vec[1], 4);
  EXPECT_EQ(vec[2], 3);
  EXPECT_EQ(vec[3], 2);
  EXPECT_EQ(vec[4], 1);
}

// Test exception when popping from empty vector
TEST_F(VectorTest, PopBackFromEmptyVector) {
  Vector<int> vec;
  EXPECT_NO_THROW(vec.pop_back());  // Should not throw, but size remains zero
  EXPECT_EQ(vec.size(), 0);
}

// Test that capacity grows correctly
TEST_F(VectorTest, CapacityGrowth) {
  Vector<int> vec;
  size_t prev_capacity = vec.capacity();
  for (int i = 0; i < 100; ++i) {
    vec.push_back(i);
    if (vec.capacity() != prev_capacity) {
      EXPECT_EQ(vec.capacity(), prev_capacity == 0 ? 1 : prev_capacity * 2);
      prev_capacity = vec.capacity();
    }
  }
}

// Test that shrink_to_fit reduces capacity
TEST_F(VectorTest, ShrinkToFitReducesCapacity) {
  s21::Vector<int> vec(50);
  EXPECT_EQ(vec.capacity(), 50);
  vec.resize(10);
  vec.shrink_to_fit();
  EXPECT_EQ(vec.capacity(), 10);
}

// Test resize() method
TEST_F(VectorTest, ResizeMethod) {
  Vector<int> vec(5);
  vec.resize(10);
  EXPECT_EQ(vec.size(), 10);
  vec.resize(3);
  EXPECT_EQ(vec.size(), 3);
}

// Test that vector can hold large number of elements
TEST_F(VectorTest, LargeVector) {
  Vector<int> vec;
  const int large_size = 100000;
  for (int i = 0; i < large_size; ++i) {
    vec.push_back(i);
  }
  EXPECT_EQ(vec.size(), large_size);
  EXPECT_EQ(vec[0], 0);
  EXPECT_EQ(vec[large_size - 1], large_size - 1);
}

// Test that vector works with move-only types
TEST_F(VectorTest, VectorWithMoveOnlyType) {
  Vector<std::unique_ptr<int>> vec;
  vec.push_back(std::make_unique<int>(10));
  vec.push_back(std::make_unique<int>(20));
  EXPECT_EQ(*vec[0], 10);
  EXPECT_EQ(*vec[1], 20);
}

// Test inserting with move semantics
TEST_F(VectorTest, InsertWithMoveSemantics) {
  Vector<std::unique_ptr<int>> vec;
  vec.push_back(std::make_unique<int>(1));
  vec.insert(vec.begin(), std::make_unique<int>(2));
  EXPECT_EQ(*vec[0], 2);
  EXPECT_EQ(*vec[1], 1);
}

// Test resizing to larger size with default values
TEST_F(VectorTest, ResizeLargerDefault) {
  s21::Vector<int> vec = {1, 2, 3};
  vec.resize(5);
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
  EXPECT_EQ(vec[3], 0);  // Default-initialized
  EXPECT_EQ(vec[4], 0);
}

// Test resizing to larger size with specified value
TEST_F(VectorTest, ResizeLargerWithValue) {
  s21::Vector<int> vec = {1, 2, 3};
  vec.resize(5, 42);
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
  EXPECT_EQ(vec[3], 42);
  EXPECT_EQ(vec[4], 42);
}

// Test resizing to smaller size
TEST_F(VectorTest, ResizeSmaller) {
  s21::Vector<int> vec = {1, 2, 3, 4, 5};
  vec.resize(3);
  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
}

// Test resizing to zero
TEST_F(VectorTest, ResizeToZero) {
  s21::Vector<int> vec = {1, 2, 3};
  vec.resize(0);
  EXPECT_TRUE(vec.empty());
  EXPECT_EQ(vec.size(), 0);
}

}  // namespace s21