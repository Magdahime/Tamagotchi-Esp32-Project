idf_component_register(SRCS "SpiDriver.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES RecyclingContainer driver)
component_compile_options(PUBLIC "-DSPIBUS_COMPONENT_TRUE=1" "-std=gnu++17")