#include "RecyclingContainer.hpp"
#include "unity.h"
#include "unity_fixture.h"
using namespace tamagotchi;

constexpr int CONTAINER_SIZE = 10;
RecyclingContainer<int, CONTAINER_SIZE> container;


TEST_GROUP(RecyclingContainerTests);

TEST_SETUP(RecyclingContainerTests)
{
}

TEST_TEAR_DOWN(RecyclingContainerTests){
 container.clear(); 
}

TEST(RecyclingContainerTests, IsEmptyInitially){
  TEST_ASSERT_EQUAL_INT(0, container.size());
  TEST_ASSERT_TRUE(container.empty());
  TEST_ASSERT_FALSE(container.full());
}

TEST(RecyclingContainerTests, IsMaxSizeSetCorrectlyInitially){
  TEST_ASSERT_EQUAL_INT(CONTAINER_SIZE, container.maxSize());
}

TEST(RecyclingContainerTests, DoesInsertWorks) {
  int number1 = 1;
  auto result = container.insert(number1);
  TEST_ASSERT_EQUAL_INT(0, result);
  TEST_ASSERT_FALSE(container.empty());
  TEST_ASSERT_FALSE(container.full());
  TEST_ASSERT_EQUAL_INT(1, container.size());
}

TEST(RecyclingContainerTests, MultipleInserting){
  int number1 = 1;
  auto result = container.insert(number1);
  auto result1 = container.insert(number1);
  auto result2 = container.insert(number1);
  TEST_ASSERT_EQUAL_INT(0, result);
  TEST_ASSERT_EQUAL_INT(1, result1);
  TEST_ASSERT_EQUAL_INT(2, result2);
  TEST_ASSERT_FALSE(container.empty());
  TEST_ASSERT_FALSE(container.full());
  TEST_ASSERT_EQUAL_INT(3, container.size());
}

TEST(RecyclingContainerTests, InsertThenRemove)  {
  int number1 = 1;
  auto result = container.insert(number1);
  container.remove(result);
  TEST_ASSERT_TRUE(container.empty());
  TEST_ASSERT_FALSE(container.full());
  TEST_ASSERT_EQUAL_INT(0, container.size());
}

TEST(RecyclingContainerTests, InsertIntoFreeSpace){
  int number1 = 1;
  container.insert(number1);
  auto result = container.insert(number1);
  container.insert(number1);
  container.remove(result);
  TEST_ASSERT_FALSE(container.empty());
  TEST_ASSERT_EQUAL_INT(2, container.size());
  result = container.insert(number1);
  TEST_ASSERT_EQUAL_INT(1, result);
  TEST_ASSERT_EQUAL_INT(3, container.size());
}
TEST(RecyclingContainerTests, GoingForwardIteratorTest){
  int number1 = 1;
  int number2 = 2;
  container.insert(number1);
  auto result = container.insert(number1);
  container.insert(number2);
  container.remove(result);
  auto begin = container.begin();
  TEST_ASSERT_EQUAL_INT(1, *begin);
  begin++;
  TEST_ASSERT_EQUAL_INT(2, *begin);
}

TEST(RecyclingContainerTests, GoingBackwardsIteratorTest) {
  int number1 = 1;
  int number2 = 2;
  container.insert(number1);
  auto result = container.insert(number1);
  container.insert(number2);
  container.remove(result);
  auto end = container.end();
  end--;
  TEST_ASSERT_EQUAL_INT(2, *end);
  end--;
  TEST_ASSERT_EQUAL_INT(1, *end);
}

TEST_GROUP_RUNNER(RecyclingContainerTests)
{
    RUN_TEST_CASE(RecyclingContainerTests, IsEmptyInitially)
    RUN_TEST_CASE(RecyclingContainerTests, IsMaxSizeSetCorrectlyInitially)
    RUN_TEST_CASE(RecyclingContainerTests, DoesInsertWorks)
    RUN_TEST_CASE(RecyclingContainerTests, MultipleInserting)
    RUN_TEST_CASE(RecyclingContainerTests, InsertThenRemove)
    RUN_TEST_CASE(RecyclingContainerTests, InsertIntoFreeSpace)
    RUN_TEST_CASE(RecyclingContainerTests, GoingForwardIteratorTest)
    RUN_TEST_CASE(RecyclingContainerTests, GoingBackwardsIteratorTest)
}