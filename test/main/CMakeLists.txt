idf_component_register(SRCS "main.cc"
                    REQUIRES unity test_utils Game Globals)
component_compile_options(-std=gnu++17)
spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)