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