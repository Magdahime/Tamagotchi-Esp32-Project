idf_component_register(SRCS "testSerializer.cc"
                    REQUIRES unity test_utils Serializer)
                    component_compile_options(-std=gnu++17)