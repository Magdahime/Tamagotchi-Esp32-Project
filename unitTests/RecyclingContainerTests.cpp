#include "../components/generic/RecyclingContainer/include/RecyclingContainer.hpp"
#include "gtest/gtest.h"

constexpr int CONTAINER_SIZE = 10;

class RecyclingContainerTest : public ::testing::Test {
 protected:
  tamagotchi::RecyclingContainer<int, CONTAINER_SIZE> container;
};

TEST_F(RecyclingContainerTest, IsEmptyInitially) {
  EXPECT_EQ(container.size(), 0);
}
