idf_component_register(SRCS "GomokuNetworking.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES MessageQueue nvs_flash)
component_compile_options(-std=gnu++17)