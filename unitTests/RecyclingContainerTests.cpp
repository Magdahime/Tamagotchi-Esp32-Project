#include "../components/generic/RecyclingContainer/include/RecyclingContainer.hpp"
#include "gtest/gtest.h"

constexpr int CONTAINER_SIZE = 10;

class RecyclingContainerTest : public ::testing::Test {
 protected:
  tamagotchi::RecyclingContainer<int, CONTAINER_SIZE> container;
};

TEST_F(RecyclingContainerTest, IsEmptyInitially) {
  EXPECT_EQ(container.size(), 0);
  EXPECT_EQ(container.empty(), true);
  EXPECT_EQ(container.full(), false);
}

TEST_F(RecyclingContainerTest, IsMaxSizeSetsCorrectlyInitally) {
  EXPECT_EQ(container.maxSize(), CONTAINER_SIZE);
}

TEST_F(RecyclingContainerTest, DoesInsertWorks) {
  int number1 = 1;
  auto result = container.insert(number1);
  EXPECT_EQ(result, 0);
  EXPECT_EQ(container.empty(), false);
  EXPECT_EQ(container.full(), false);
  EXPECT_EQ(container.size(), 1);
}

TEST_F(RecyclingContainerTest, MultipleInserting) {
  int number1 = 1;
  auto result = container.insert(number1);
  auto result1 = container.insert(number1);
  auto result2 = container.insert(number1);
  EXPECT_EQ(result, 0);
  EXPECT_EQ(result1, 1);
  EXPECT_EQ(result2, 2);
  EXPECT_EQ(container.empty(), false);
  EXPECT_EQ(container.full(), false);
  EXPECT_EQ(container.size(), 3);
}

TEST_F(RecyclingContainerTest, InsertThenRemove) {
  int number1 = 1;
  auto result = container.insert(number1);
  container.remove(result);
  EXPECT_EQ(container.empty(), true);
  EXPECT_EQ(container.full(), false);
  EXPECT_EQ(container.size(), 0);
}


TEST_F(RecyclingContainerTest, InsertIntoFreeSpace) {
  int number1 = 1;
  container.insert(number1);
  auto result = container.insert(number1);
  container.insert(number1);
  container.remove(result);
  EXPECT_EQ(container.empty(), false);
  EXPECT_EQ(container.size(), 2);
  result = container.insert(number1);
  EXPECT_EQ(result, 1);
  EXPECT_EQ(container.size(), 3);
}

TEST_F(RecyclingContainerTest, GoingForwardIteratorTest) {
  int number1 = 1;
  int number2 = 2;
  container.insert(number1);
  auto result = container.insert(number1);
  container.insert(number2);
  container.remove(result);
  auto begin = container.begin();
  EXPECT_EQ(*begin, 1);
  begin++;
  EXPECT_EQ(*begin, 2);
}

TEST_F(RecyclingContainerTest, GoingBackwardsIteratorTest) {
  int number1 = 1;
  int number2 = 2;
  container.insert(number1);
  auto result = container.insert(number1);
  container.insert(number2);
  container.remove(result);
  auto end = container.end();
  end--;
  EXPECT_EQ(*end, 2);
  end--;
  EXPECT_EQ(*end, 1);
}
