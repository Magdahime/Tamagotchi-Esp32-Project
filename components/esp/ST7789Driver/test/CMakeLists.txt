idf_component_register(SRCS "testST7789Driver.cc" "testST7789Utils.cc"
                       INCLUDE_DIRS "include"
                       REQUIRES unity test_utils ST7789Driver)