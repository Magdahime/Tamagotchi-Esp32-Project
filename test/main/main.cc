
#include <stdio.h>
#include <string.h>

#include "unity.h"


static void print_banner(const char* text) {
  printf("\n#### %s #####\n\n", text);
}


extern "C" void app_main() {
  print_banner("Running all the registered tests");
  UNITY_BEGIN();
  unity_run_tests_by_tag("[filled_polygons]", false);
  UNITY_END();

  // UNITY_BEGIN();
  // unity_run_all_tests();
  // UNITY_END();
}