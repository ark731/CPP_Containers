#include <gtest/gtest.h>

#include "../s21_containers.h"

namespace s21 {

class MapTest : public ::testing::Test {
 protected:
  Map<int, std::string> default_map;
  Map<int, std::string, std::greater<int>> custom_map;

  void SetUp() override {
    // Prepopulate default_map with some values
    default_map.insert({1, "one"});
    default_map.insert({2, "two"});
    default_map.insert({3, "three"});

    // Prepopulate custom_map with reverse order
    custom_map.insert({3, "three"});
    custom_map.insert({2, "two"});
    custom_map.insert({1, "one"});
  }
};

TEST_F(MapTest, DefaultConstructor) {
  Map<int, std::string> map;
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0);
}

TEST_F(MapTest, CopyConstructor) {
  Map<int, std::string> copy_map(default_map);
  EXPECT_EQ(copy_map.size(), default_map.size());
  EXPECT_EQ(copy_map[1], "one");
}

TEST_F(MapTest, MoveConstructor) {
  Map<int, std::string> temp_map({{1, "one"}, {2, "two"}});
  Map<int, std::string> moved_map(std::move(temp_map));
  EXPECT_EQ(moved_map.size(), 2);
  EXPECT_TRUE(temp_map.empty());
}

TEST_F(MapTest, AssignmentOperator) {
  Map<int, std::string> assign_map;
  assign_map = default_map;
  EXPECT_EQ(assign_map.size(), default_map.size());
  EXPECT_EQ(assign_map[1], "one");
}

TEST_F(MapTest, BeginAndEnd) {
  auto it = default_map.begin();
  EXPECT_EQ(it->first, 1);
  ++it;
  EXPECT_EQ(it->first, 2);
}

TEST_F(MapTest, ConstIterator) {
  const auto& const_map = default_map;
  auto it = const_map.begin();
  EXPECT_EQ(it->first, 1);
  ++it;
  EXPECT_EQ(it->first, 2);
}

TEST_F(MapTest, EmptyAndSize) {
  EXPECT_FALSE(default_map.empty());
  EXPECT_EQ(default_map.size(), 3);

  Map<int, std::string> empty_map;
  EXPECT_TRUE(empty_map.empty());
  EXPECT_EQ(empty_map.size(), 0);
}

TEST_F(MapTest, InsertSingleElement) {
  auto [it, success] = default_map.insert({4, "four"});
  EXPECT_TRUE(success);
  EXPECT_EQ(it->first, 4);
  EXPECT_EQ(default_map.size(), 4);
}

TEST_F(MapTest, InsertMany) {
  auto results = default_map.insert_many(std::make_pair(4, "four"),
                                         std::make_pair(5, "five"));
  EXPECT_EQ(results.size(), 2);
  EXPECT_TRUE(results[0].second);
  EXPECT_TRUE(results[1].second);
  EXPECT_EQ(default_map.size(), 5);
}

TEST_F(MapTest, EraseByIterator) {
  auto it = default_map.find(2);
  default_map.erase(it);
  EXPECT_EQ(default_map.size(), 2);
  EXPECT_EQ(default_map.find(2), default_map.end());
}

TEST_F(MapTest, EraseByKey) {
  EXPECT_EQ(default_map.erase(2), 1);
  EXPECT_EQ(default_map.size(), 2);
  EXPECT_EQ(default_map.erase(99), 0);  // Non-existent key
}

TEST_F(MapTest, AtAndOperatorSquareBrackets) {
  EXPECT_EQ(default_map.at(1), "one");
  EXPECT_THROW(default_map.at(99), std::out_of_range);

  default_map[4] = "four";
  EXPECT_EQ(default_map[4], "four");
}

TEST_F(MapTest, Find) {
  auto it = default_map.find(2);
  EXPECT_NE(it, default_map.end());
  EXPECT_EQ(it->second, "two");

  auto non_existent = default_map.find(99);
  EXPECT_EQ(non_existent, default_map.end());
}

TEST_F(MapTest, Count) {
  EXPECT_EQ(default_map.count(1), 1);
  EXPECT_EQ(default_map.count(99), 0);
}

TEST_F(MapTest, Bounds) {
  auto lower = default_map.lower_bound(2);
  EXPECT_EQ(lower->first, 2);

  auto upper = default_map.upper_bound(2);
  EXPECT_EQ(upper->first, 3);
}

TEST_F(MapTest, EqualRange) {
  auto [lower, upper] = default_map.equal_range(2);
  EXPECT_EQ(lower->first, 2);
  EXPECT_EQ(upper->first, 3);
}

TEST_F(MapTest, KeyCompAndValueComp) {
  EXPECT_TRUE(default_map.key_comp()(1, 2));
  EXPECT_TRUE(default_map.value_comp()({1, "one"}, {2, "two"}));
}

TEST(MapTest, InsertManyMixedValidity) {
  Map<int, std::string> map;
  auto results =
      map.insert_many(std::make_pair(1, "one"), std::make_pair(2, "two"),
                      std::make_pair(2, "duplicate"));

  ASSERT_EQ(results.size(), 3);
  EXPECT_TRUE(results[0].second);   // Successfully inserted
  EXPECT_TRUE(results[1].second);   // Successfully inserted
  EXPECT_FALSE(results[2].second);  // Duplicate key, no insertion
  EXPECT_EQ(map.size(), 2);
}

TEST(MapTest, IteratorValidityAfterErase) {
  Map<int, std::string> map;
  map.insert({1, "one"});
  map.insert({2, "two"});
  map.insert({3, "three"});

  auto it = map.begin();
  map.erase(it);  // Erase the first element
  it = map.begin();
  EXPECT_EQ(it->first, 2);

  map.erase(it);  // Erase the next element
  it = map.begin();
  EXPECT_EQ(it->first, 3);

  map.erase(it);  // Erase the last element
  EXPECT_TRUE(map.empty());
}

TEST(MapTest, EqualRangeClosestKeys) {
  Map<int, std::string> map;
  map.insert({1, "one"});
  map.insert({3, "three"});
  map.insert({5, "five"});

  auto range =
      map.equal_range(4);  // Key doesn't exist, should span closest match
  EXPECT_EQ(range.first->first, 5);   // Lower bound
  EXPECT_EQ(range.second->first, 5);  // Upper bound
}

TEST(MapTest, StressLargeDataset) {
  constexpr int n = 100000;
  Map<int, int> map;
  for (int i = 0; i < n; ++i) {
    map.insert({i, i * 2});
  }

  EXPECT_EQ(map.size(), n);
  for (int i = 0; i < n; ++i) {
    EXPECT_EQ(map.at(i), i * 2);
  }
}

TEST(MapTest, NestedMapMoveSemantics) {
  using NestedMap = Map<int, Map<int, std::string>>;
  NestedMap outer;
  outer[1][1] = "one";
  outer[1][2] = "two";

  NestedMap moved_outer(std::move(outer));
  EXPECT_TRUE(outer.empty());
  EXPECT_EQ(moved_outer.size(), 1);
  EXPECT_EQ(moved_outer[1].size(), 2);
  EXPECT_EQ(moved_outer[1].at(1), "one");
  EXPECT_EQ(moved_outer[1].at(2), "two");
}

TEST(MapTest, CustomComparatorReverseOrder) {
  Map<int, std::string, std::greater<int>> map;  // Reverse order
  map.insert({1, "one"});
  map.insert({2, "two"});
  map.insert({3, "three"});

  auto it = map.begin();
  EXPECT_EQ(it->first, 3);
  ++it;
  EXPECT_EQ(it->first, 2);
  ++it;
  EXPECT_EQ(it->first, 1);
}

TEST(MapTest, ClearPopulatedMap) {
  Map<int, std::string> map;
  map.insert({1, "one"});
  map.insert({2, "two"});
  map.insert({3, "three"});

  map.clear();
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0);
  EXPECT_EQ(map.begin(), map.end());
}

TEST(MapTest, EraseNonExistentKey) {
  Map<int, std::string> map;
  map.insert({1, "one"});
  map.insert({2, "two"});

  size_t erased_count = map.erase(3);  // Key 3 doesn't exist
  EXPECT_EQ(erased_count, 0);
  EXPECT_EQ(map.size(), 2);
}

TEST(MapTest, LowerAndUpperBounds) {
  Map<int, std::string> map;
  map.insert({1, "one"});
  map.insert({3, "three"});
  map.insert({5, "five"});

  auto lb = map.lower_bound(4);
  auto ub = map.upper_bound(4);

  EXPECT_EQ(lb->first, 5);  // Lower bound should point to 5
  EXPECT_EQ(ub->first, 5);  // Upper bound should point to 5
}

TEST(MapTest, AtThrowsOnNonExistentKey) {
  Map<int, std::string> map;
  map.insert({1, "one"});
  map.insert({2, "two"});

  EXPECT_THROW(map.at(3), std::out_of_range);  // Key 3 doesn't exist
  EXPECT_NO_THROW(map.at(2));                  // Key 2 exists
}

}  // namespace s21
