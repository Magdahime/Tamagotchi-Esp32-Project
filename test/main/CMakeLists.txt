idf_component_register(SRCS "main.cc"
                    REQUIRES unity test_utils)
component_compile_options(-std=gnu++17)
spiffs_create_partition_image(storage ../spiffs SPIFFS_IMAGE_FLASH_IN_PROJECT)