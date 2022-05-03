idf_component_register(SRCS "main.cc" REQUIRES nvs_flash Game Joystick GomokuNetworking)
component_compile_options(-std=gnu++17)
spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)