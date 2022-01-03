
#include <stdio.h>
#include <string.h>

#include "unity.h"

extern "C" {
#include "unity_fixture.h"
}

static void run_all_tests(void) {
  RUN_TEST_GROUP(RecyclingContainerTests);
  RUN_TEST_GROUP(EspGLUtilsTests);
  RUN_TEST_GROUP(EspGLShapesTests);
  RUN_TEST_GROUP(SPIFFSDriverTests);
  RUN_TEST_GROUP(EspGLFontLoaderTests);
  RUN_TEST_GROUP(EspGLTextTests);
}

static void print_banner(const char* text) {
  printf("\n#### %s #####\n\n", text);
}

extern "C" void app_main() {
  print_banner("Running all the registered tests");
  UNITY_MAIN_FUNC(run_all_tests);
}