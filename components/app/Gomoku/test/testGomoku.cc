#include "Gomoku.hpp"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(GomokuTests);

TEST_SETUP(GomokuTests) {}

TEST_TEAR_DOWN(GomokuTests) {}

TEST(GomokuTests, CreateGomokuBoard) {
  tamagotchi::App::Gomoku::Gomoku<3, 3> gomoku;
  TEST_ASSERT_EQUAL_INT(9, gomoku.board().size());
  TEST_ASSERT_EQUAL_INT(0, gomoku.board().front());
}

TEST(GomokuTests, MakingOutOfBoundsMove) {
  tamagotchi::App::Gomoku::Gomoku<3, 3> gomoku;
  try {
    gomoku.markMove(1, {3, 4});
  } catch (const std::runtime_error& e) {
    TEST_ASSERT_EQUAL_STRING("Move coordinates are out of bounds: (3, 4)",
                             e.what());
    TEST_PASS();
  }

  TEST_FAIL_MESSAGE("Expected throw()");
}

TEST(GomokuTests, UsingTwoTimesTheSameTile) {
  tamagotchi::App::Gomoku::Gomoku<3, 3> gomoku;
  try {
    gomoku.markMove(1, {2, 2});
    gomoku.markMove(1, {2, 2});
  } catch (const std::runtime_error& e) {
    TEST_ASSERT_EQUAL_STRING("This tile has been already used: (2, 2)",
                             e.what());
    TEST_PASS();
  }

  TEST_FAIL_MESSAGE("Expected throw()");
}

TEST(GomokuTests, MarkingWithZero) {
  tamagotchi::App::Gomoku::Gomoku<3, 3> gomoku;
  try {
    gomoku.markMove(0, {2, 2});
  } catch (const std::runtime_error& e) {
    TEST_ASSERT_EQUAL_STRING("Marking sign shouldn't be zero!", e.what());
    TEST_PASS();
  }

  TEST_FAIL_MESSAGE("Expected throw()");
}

TEST(GomokuTests, MarkingGomokuBoard) {
  tamagotchi::App::Gomoku::Gomoku<3, 3> gomoku;
  gomoku.markMove(1, {2, 2});
  TEST_ASSERT_EQUAL_INT(1, gomoku.board()[8]);
}

TEST(GomokuTests, HorizontalLineCheck) {
  tamagotchi::App::Gomoku::Gomoku<5, 5> gomoku;
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {0, 0}));
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {1, 0}));
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {2, 0}));
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {3, 0}));
  TEST_ASSERT_EQUAL_INT(1, gomoku.markMove(1, {4, 0}));
}

TEST(GomokuTests, VerticalLineCheck) {
  tamagotchi::App::Gomoku::Gomoku<5, 5> gomoku;
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {0, 0}));
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {0, 1}));
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {0, 2}));
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {0, 3}));
  TEST_ASSERT_EQUAL_INT(1, gomoku.markMove(1, {0, 4}));
}

TEST(GomokuTests, DiagonalLineCheck) {
  tamagotchi::App::Gomoku::Gomoku<5, 5> gomoku;
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {0, 0}));
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {1, 1}));
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {2, 2}));
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {3, 3}));
  TEST_ASSERT_EQUAL_INT(1, gomoku.markMove(1, {4, 4}));
}

TEST(GomokuTests, DiagonalLineCheck2) {
  tamagotchi::App::Gomoku::Gomoku<5, 5> gomoku;
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {4, 0}));
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {3, 1}));
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {2, 2}));
  TEST_ASSERT_EQUAL_INT(0, gomoku.markMove(1, {1, 3}));
  TEST_ASSERT_EQUAL_INT(1, gomoku.markMove(1, {0, 4}));
}

TEST_GROUP_RUNNER(GomokuTests) {
  RUN_TEST_CASE(GomokuTests, CreateGomokuBoard)
  RUN_TEST_CASE(GomokuTests, MakingOutOfBoundsMove)
  RUN_TEST_CASE(GomokuTests, UsingTwoTimesTheSameTile)
  RUN_TEST_CASE(GomokuTests, MarkingGomokuBoard)
  RUN_TEST_CASE(GomokuTests, HorizontalLineCheck)
  RUN_TEST_CASE(GomokuTests, VerticalLineCheck)
  RUN_TEST_CASE(GomokuTests, DiagonalLineCheck)
  RUN_TEST_CASE(GomokuTests, DiagonalLineCheck2)
}