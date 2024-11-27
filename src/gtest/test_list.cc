#include <gtest/gtest.h>

#include <utility>

#include "../s21_containers.h"

namespace s21 {
class ListTest : public ::testing::Test {
 protected:
  // Setup and teardown (if needed)
  ListTest() = default;
  ~ListTest() = default;
};

// Test default constructor
TEST_F(ListTest, DefaultConstructor) {
  List<int> lst;
  EXPECT_TRUE(lst.empty());
  EXPECT_EQ(lst.size(), 0);
}

// Test constructor with initializer list
TEST_F(ListTest, InitializerListConstructor) {
  List<int> lst = {1, 2, 3, 4, 5};
  EXPECT_EQ(lst.size(), 5);
  EXPECT_EQ(lst.front(), 1);
  EXPECT_EQ(lst.back(), 5);
}

// Test copy constructor
TEST_F(ListTest, CopyConstructor) {
  List<int> lst1 = {1, 2, 3, 4, 5};
  List<int> lst2 = lst1;
  EXPECT_EQ(lst2.size(), 5);
  EXPECT_EQ(lst2.front(), 1);
  EXPECT_EQ(lst2.back(), 5);
}

// Test move constructor
TEST_F(ListTest, MoveConstructor) {
  List<int> lst1 = {1, 2, 3, 4, 5};
  List<int> lst2 = std::move(lst1);
  EXPECT_EQ(lst2.size(), 5);
  EXPECT_EQ(lst2.front(), 1);
  EXPECT_EQ(lst2.back(), 5);
  EXPECT_TRUE(lst1.empty());
}

// Test copy assignment operator
TEST_F(ListTest, CopyAssignmentOperator) {
  List<int> lst1 = {1, 2, 3, 4, 5};
  List<int> lst2;
  lst2 = lst1;
  EXPECT_EQ(lst2.size(), 5);
  EXPECT_EQ(lst2.front(), 1);
  EXPECT_EQ(lst2.back(), 5);
}

// Test move assignment operator
TEST_F(ListTest, MoveAssignmentOperator) {
  List<int> lst1 = {1, 2, 3, 4, 5};
  List<int> lst2;
  lst2 = std::move(lst1);
  EXPECT_EQ(lst2.size(), 5);
  EXPECT_EQ(lst2.front(), 1);
  EXPECT_EQ(lst2.back(), 5);
  EXPECT_TRUE(lst1.empty());
}

// Test element access: front() and back()
TEST_F(ListTest, ElementAccess) {
  List<int> lst = {10, 20, 30, 40, 50};
  EXPECT_EQ(lst.front(), 10);
  EXPECT_EQ(lst.back(), 50);
}

// Test empty() method
TEST_F(ListTest, EmptyMethod) {
  List<int> lst_empty;
  EXPECT_TRUE(lst_empty.empty());

  List<int> lst_non_empty = {1};
  EXPECT_FALSE(lst_non_empty.empty());
}

// Test size() method
TEST_F(ListTest, SizeMethod) {
  List<int> lst_empty;
  EXPECT_EQ(lst_empty.size(), 0);

  List<int> lst = {1, 2, 3};
  EXPECT_EQ(lst.size(), 3);
}

// Test push_back() method
TEST_F(ListTest, PushBackMethod) {
  List<int> lst;
  lst.push_back(10);
  lst.push_back(20);
  lst.push_back(30);
  EXPECT_EQ(lst.size(), 3);
  EXPECT_EQ(lst.back(), 30);
}

// Test push_front() method
TEST_F(ListTest, PushFrontMethod) {
  List<int> lst;
  lst.push_front(10);
  lst.push_front(20);
  lst.push_front(30);
  EXPECT_EQ(lst.size(), 3);
  EXPECT_EQ(lst.front(), 30);
}

// Test pop_back() method
TEST_F(ListTest, PopBackMethod) {
  List<int> lst = {1, 2, 3};
  lst.pop_back();
  EXPECT_EQ(lst.size(), 2);
  EXPECT_EQ(lst.back(), 2);
}

// Test pop_front() method
TEST_F(ListTest, PopFrontMethod) {
  List<int> lst = {1, 2, 3};
  lst.pop_front();
  EXPECT_EQ(lst.size(), 2);
  EXPECT_EQ(lst.front(), 2);
}

// Test erase() method
TEST_F(ListTest, EraseMethod) {
  List<int> lst = {1, 2, 3, 4, 5};
  auto it = lst.begin();
  ++it;  // Points to 2
  lst.erase(it);
  EXPECT_EQ(lst.size(), 4);
  EXPECT_EQ(lst.front(), 1);
  EXPECT_EQ(lst.back(), 5);
}

// Test insert() method
TEST_F(ListTest, InsertMethod) {
  List<int> lst = {1, 2, 3};
  auto it = lst.begin();
  ++it;  // Points to 2
  lst.insert(it, 10);
  EXPECT_EQ(lst.size(), 4);
  EXPECT_EQ(lst.front(), 1);
  EXPECT_EQ(lst.back(), 3);
  EXPECT_EQ(*(++it), 3);
}

// Test clear() method
TEST_F(ListTest, ClearMethod) {
  List<int> lst = {1, 2, 3};
  lst.clear();
  EXPECT_TRUE(lst.empty());
  EXPECT_EQ(lst.size(), 0);
}

// Test swap() method
TEST_F(ListTest, SwapMethod) {
  List<int> lst1 = {1, 2, 3};
  List<int> lst2 = {4, 5, 6};
  lst1.swap(lst2);
  EXPECT_EQ(lst1.size(), 3);
  EXPECT_EQ(lst2.size(), 3);
  EXPECT_EQ(lst1.front(), 4);
  EXPECT_EQ(lst2.front(), 1);
}

// Test insert_many_back() method
TEST_F(ListTest, InsertManyBack) {
  List<int> lst = {1, 2, 3};
  lst.insert_many_back(4, 5, 6);
  int expected[] = {1, 2, 3, 4, 5, 6};
  int i = 0;
  for (auto val : lst) {
    EXPECT_EQ(val, expected[i++]);
  }
}

// Test insert_many_front() method
TEST_F(ListTest, InsertManyFront) {
  List<int> lst = {4, 5, 6};
  lst.insert_many_front(1, 2, 3);
  int expected[] = {1, 2, 3, 4, 5, 6};
  int i = 0;
  for (auto val : lst) {
    EXPECT_EQ(val, expected[i++]);
  }
}

// Test iterator begin() and end()
TEST_F(ListTest, IteratorBeginEnd) {
  List<int> lst = {10, 20, 30};
  int expected_value = 10;
  for (auto it = lst.begin(); it != lst.end(); ++it) {
    EXPECT_EQ(*it, expected_value);
    expected_value += 10;
  }
}

// Test const_iterator begin() and end()
TEST_F(ListTest, ConstIteratorBeginEnd) {
  const List<int> lst = {10, 20, 30};
  int expected_value = 10;
  for (auto it = lst.begin(); it != lst.end(); ++it) {
    EXPECT_EQ(*it, expected_value);
    expected_value += 10;
  }
}

// Test reverse iteration
TEST_F(ListTest, ReverseIteration) {
  List<int> lst = {10, 20, 30, 40, 50};
  int expected_value = 50;
  for (auto it = lst.end(); it != lst.begin();) {
    --it;
    EXPECT_EQ(*it, expected_value);
    expected_value -= 10;
  }
}

// Test insert at beginning
TEST_F(ListTest, InsertAtBeginning) {
  List<int> lst = {2, 3, 4};
  lst.insert(lst.begin(), 1);
  EXPECT_EQ(lst.front(), 1);
  EXPECT_EQ(lst.size(), 4);
}

// Test insert at end
TEST_F(ListTest, InsertAtEnd) {
  List<int> lst = {1, 2, 3};
  lst.insert(lst.end(), 4);
  EXPECT_EQ(lst.back(), 4);
  EXPECT_EQ(lst.size(), 4);
}

// Test erase first element
TEST_F(ListTest, EraseFirstElement) {
  List<int> lst = {1, 2, 3};
  lst.erase(lst.begin());
  EXPECT_EQ(lst.front(), 2);
  EXPECT_EQ(lst.size(), 2);
}

// Test erase last element
TEST_F(ListTest, EraseLastElement) {
  List<int> lst = {1, 2, 3};
  auto it = lst.end();
  --it;  // Points to 3
  lst.erase(it);
  EXPECT_EQ(lst.back(), 2);
  EXPECT_EQ(lst.size(), 2);
}

// Test erase all elements
TEST_F(ListTest, EraseAllElements) {
  List<int> lst = {1, 2, 3};
  lst.erase(lst.begin());
  lst.erase(lst.begin());
  lst.erase(lst.begin());
  EXPECT_TRUE(lst.empty());
  EXPECT_EQ(lst.size(), 0);
}

// Test front() and back() on empty list
TEST_F(ListTest, FrontBackOnEmptyList) {
  List<int> lst;
  EXPECT_THROW(lst.front(), std::out_of_range);
  EXPECT_THROW(lst.back(), std::out_of_range);
}

// Test pop_back() and pop_front() on empty list
TEST_F(ListTest, PopOnEmptyList) {
  List<int> lst;
  lst.pop_back();  // Should not throw, but size remains zero
  lst.pop_front();
  EXPECT_TRUE(lst.empty());
  EXPECT_EQ(lst.size(), 0);
}

// Test insert() on empty list
TEST_F(ListTest, InsertOnEmptyList) {
  List<int> lst;
  lst.insert(lst.begin(), 1);
  EXPECT_EQ(lst.front(), 1);
  EXPECT_EQ(lst.size(), 1);
}

// Test erase() on empty list
TEST_F(ListTest, EraseOnEmptyList) {
  List<int> lst;
  EXPECT_THROW(lst.erase(lst.begin()), std::out_of_range);
}

// Test iterator increment at end
TEST_F(ListTest, IteratorIncrementAtEnd) {
  List<int> lst = {1, 2, 3};
  auto it = lst.end();
  EXPECT_NO_THROW(++it);  // Should remain at end()
  EXPECT_TRUE(it == lst.end());
}

// Test iterator decrement at begin
TEST_F(ListTest, IteratorDecrementAtBegin) {
  List<int> lst = {1, 2, 3};
  auto it = lst.begin();
  EXPECT_NO_THROW(--it);
}

// Test modifying elements through iterator
TEST_F(ListTest, ModifyThroughIterator) {
  List<int> lst = {1, 2, 3};
  for (auto it = lst.begin(); it != lst.end(); ++it) {
    *it *= 2;
  }
  int expected[] = {2, 4, 6};
  int i = 0;
  for (auto val : lst) {
    EXPECT_EQ(val, expected[i++]);
  }
}

// Test const correctness of iterators
TEST_F(ListTest, ConstCorrectnessIterator) {
  const List<int> lst = {1, 2, 3};
  auto it = lst.begin();
  EXPECT_EQ(*it, 1);
  // Uncommenting the next line should cause a compilation error
  // *it = 10;
}

// Test exception safety in insert()
TEST_F(ListTest, InsertExceptionSafety) {
  struct ExceptionThrowingType {
    int value;
    ExceptionThrowingType(int v) : value(v) {
      if (v == -1) throw std::runtime_error("Exception in constructor");
    }
    ExceptionThrowingType(const ExceptionThrowingType& other)
        : value(other.value) {
      if (value == -1)
        throw std::runtime_error("Exception in copy constructor");
    }
  };

  List<ExceptionThrowingType> lst = {1, 2, 3};
  try {
    lst.insert(lst.begin(), -1);
  } catch (const std::runtime_error&) {
    // Exception caught
  }
  // List should remain unchanged
  EXPECT_EQ(lst.size(), 3);
  int expected[] = {1, 2, 3};
  int i = 0;
  for (auto it = lst.begin(); it != lst.end(); ++it) {
    EXPECT_EQ(it->value, expected[i++]);
  }
}

// Test swap with empty list
TEST_F(ListTest, SwapWithEmptyList) {
  List<int> lst1 = {1, 2, 3};
  List<int> lst2;
  lst1.swap(lst2);
  EXPECT_TRUE(lst1.empty());
  EXPECT_EQ(lst2.size(), 3);
  int expected[] = {1, 2, 3};
  int i = 0;
  for (auto val : lst2) {
    EXPECT_EQ(val, expected[i++]);
  }
}

// Test using standard algorithms (e.g., std::reverse)
TEST_F(ListTest, UseStdReverse) {
  List<int> lst = {1, 2, 3, 4, 5};
  std::reverse(lst.begin(), lst.end());
  int expected[] = {5, 4, 3, 2, 1};
  int i = 0;
  for (auto val : lst) {
    EXPECT_EQ(val, expected[i++]);
  }
}

// Test moving elements between lists
TEST_F(ListTest, MoveElementsBetweenLists) {
  List<int> lst1 = {1, 2, 3};
  List<int> lst2;
  lst2 = std::move(lst1);
  EXPECT_TRUE(lst1.empty());
  EXPECT_EQ(lst2.size(), 3);
  int expected[] = {1, 2, 3};
  int i = 0;
  for (auto val : lst2) {
    EXPECT_EQ(val, expected[i++]);
  }
}

// Test list with move-only types (e.g., std::unique_ptr)
TEST_F(ListTest, ListWithMoveOnlyType) {
  List<std::unique_ptr<int>> lst;
  lst.push_back(std::make_unique<int>(1));
  lst.push_back(std::make_unique<int>(2));
  EXPECT_EQ(*lst.front(), 1);
  EXPECT_EQ(*lst.back(), 2);
}

// Test inserting move-only types
TEST_F(ListTest, InsertMoveOnlyType) {
  List<std::unique_ptr<int>> lst;
  lst.insert(lst.begin(), std::make_unique<int>(1));
  lst.insert(lst.end(), std::make_unique<int>(2));
  EXPECT_EQ(*lst.front(), 1);
  EXPECT_EQ(*lst.back(), 2);
}

// Test inserting at invalid position
TEST_F(ListTest, InsertAtInvalidPosition) {
  List<int> lst = {1, 2, 3};
  auto it = lst.end();
  ++it;  // Invalid iterator
  EXPECT_NO_THROW(lst.insert(it, 4));
}

// Test erasing at invalid position
TEST_F(ListTest, EraseAtInvalidPosition) {
  List<int> lst = {1, 2, 3};
  auto it = lst.end();
  ++it;  // Invalid iterator
  EXPECT_THROW(lst.erase(it), std::out_of_range);
}

// Test iterator invalidation after insert
TEST_F(ListTest, IteratorInvalidationAfterInsert) {
  List<int> lst = {1, 2, 3};
  auto it = lst.begin();
  lst.insert(lst.begin(), 0);
  // 'it' should still be valid and point to 1
  EXPECT_EQ(*it, 1);
}

// Test assign operator self-assignment
TEST_F(ListTest, SelfAssignment) {
  List<int> lst = {1, 2, 3};
  lst = lst;
  EXPECT_EQ(lst.size(), 3);
  int expected[] = {1, 2, 3};
  int i = 0;
  for (auto val : lst) {
    EXPECT_EQ(val, expected[i++]);
  }
}

// Test destructor (implicit test for memory leaks)
// Note: This is not directly testable via code, but we ensure no memory leaks
// occur
TEST_F(ListTest, DestructorTest) {
  List<int>* lst = new List<int>({1, 2, 3});
  delete lst;
  // If running under a memory checker like Valgrind, ensure no leaks are
  // reported
}

// Test clear method on empty list
TEST_F(ListTest, ClearEmptyList) {
  List<int> lst;
  lst.clear();
  EXPECT_TRUE(lst.empty());
}

// Test push_back and pop_back consistency
TEST_F(ListTest, PushBackPopBackConsistency) {
  List<int> lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);
  EXPECT_EQ(lst.back(), 3);
  lst.pop_back();
  EXPECT_EQ(lst.back(), 2);
  lst.pop_back();
  EXPECT_EQ(lst.back(), 1);
  lst.pop_back();
  EXPECT_TRUE(lst.empty());
}

// Test push_front and pop_front consistency
TEST_F(ListTest, PushFrontPopFrontConsistency) {
  List<int> lst;
  lst.push_front(1);
  lst.push_front(2);
  lst.push_front(3);
  EXPECT_EQ(lst.front(), 3);
  lst.pop_front();
  EXPECT_EQ(lst.front(), 2);
  lst.pop_front();
  EXPECT_EQ(lst.front(), 1);
  lst.pop_front();
  EXPECT_TRUE(lst.empty());
}

// Test large number of elements
TEST_F(ListTest, LargeNumberOfElements) {
  List<int> lst;
  const int num_elements = 10000;
  for (int i = 0; i < num_elements; ++i) {
    lst.push_back(i);
  }
  EXPECT_EQ(lst.size(), num_elements);
  int i = 0;
  for (auto val : lst) {
    EXPECT_EQ(val, i++);
  }
}

// Test custom object with copy and move semantics
struct CustomObject {
  int x;
  CustomObject(int val) : x(val) {}
  CustomObject(const CustomObject& other) : x(other.x) {}
  CustomObject(CustomObject&& other) noexcept : x(other.x) { other.x = 0; }
  CustomObject& operator=(const CustomObject& other) {
    x = other.x;
    return *this;
  }
  CustomObject& operator=(CustomObject&& other) noexcept {
    x = other.x;
    other.x = 0;
    return *this;
  }
};

TEST_F(ListTest, ListOfCustomObjects) {
  List<CustomObject> lst;
  lst.push_back(CustomObject(1));
  lst.push_back(CustomObject(2));
  lst.push_back(CustomObject(3));
  int expected[] = {1, 2, 3};
  int i = 0;
  for (auto& obj : lst) {
    EXPECT_EQ(obj.x, expected[i++]);
  }
}

// Test list of lists
TEST_F(ListTest, ListOfLists) {
  List<List<int>> lst_of_lsts;
  lst_of_lsts.push_back(List<int>({1, 2}));
  lst_of_lsts.push_back(List<int>({3, 4}));
  auto it = lst_of_lsts.begin();
  int expected_first[] = {1, 2};
  int expected_second[] = {3, 4};
  int i = 0;
  for (auto val : *it) {
    EXPECT_EQ(val, expected_first[i++]);
  }
  ++it;
  i = 0;
  for (auto val : *it) {
    EXPECT_EQ(val, expected_second[i++]);
  }
}

// Test assignment operator with different sizes
TEST_F(ListTest, AssignmentOperatorDifferentSizes) {
  List<int> lst1 = {1, 2, 3, 4, 5};
  List<int> lst2 = {6, 7};
  lst1 = lst2;
  EXPECT_EQ(lst1.size(), 2);
  int expected[] = {6, 7};
  int i = 0;
  for (auto val : lst1) {
    EXPECT_EQ(val, expected[i++]);
  }
}

// Test assigning to elements via iterator
TEST_F(ListTest, AssignViaIterator) {
  List<int> lst = {1, 2, 3};
  auto it = lst.begin();
  *it = 10;
  ++it;
  *it = 20;
  ++it;
  *it = 30;
  int expected[] = {10, 20, 30};
  int i = 0;
  for (auto val : lst) {
    EXPECT_EQ(val, expected[i++]);
  }
}

// Test exception when dereferencing end()
TEST_F(ListTest, DereferenceEndIterator) {
  List<int> lst = {1, 2, 3};
  auto it = lst.end();
  EXPECT_THROW(*it, std::out_of_range);
}

// Test exception when decrementing begin()
TEST_F(ListTest, DecrementBeginIterator) {
  List<int> lst = {1, 2, 3};
  auto it = lst.begin();
  EXPECT_NO_THROW(--it);
}

// Test exception when incrementing end()
TEST_F(ListTest, IncrementEndIterator) {
  List<int> lst = {1, 2, 3};
  auto it = lst.end();
  ++it;  // Should stay at end()
  EXPECT_TRUE(it == lst.end());
}

// Test exception when erasing using end()
TEST_F(ListTest, EraseEndIterator) {
  List<int> lst = {1, 2, 3};
  EXPECT_THROW(lst.erase(lst.end()), std::out_of_range);
}

// Test exception when accessing element via invalid iterator
TEST_F(ListTest, AccessViaInvalidIterator) {
  List<int> lst = {1, 2, 3};
  auto it = lst.begin();
  lst.erase(it);
  EXPECT_NO_THROW(*it);
}

}  // namespace s21