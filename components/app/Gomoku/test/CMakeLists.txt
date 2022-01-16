idf_component_register(SRCS "testGomoku.cc"
                       REQUIRES unity test_utils Gomoku)
                       component_compile_options(-std=gnu++17)