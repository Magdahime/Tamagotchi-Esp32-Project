
#include <stdio.h>
#include <string.h>

#include "unity_fixture.h"
#include "unity_fixture_extras.h"

static void run_all_tests(void)
{
    RUN_TEST_GROUP(ST7789VWDriverTests);
    RUN_TEST_GROUP(RecyclingContainerTests);
    RUN_TEST_GROUP(ST7789VWUtilsTests);
}

static void print_banner(const char* text) {
  printf("\n#### %s #####\n\n", text);
}

extern "C" void app_main() {
  print_banner("Running all the registered tests");
  UNITY_MAIN_FUNC(run_all_tests);
}