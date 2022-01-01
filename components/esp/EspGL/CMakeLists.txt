idf_component_register(SRCS "EspGLUtils.cc" "Graphics/Font/EspGLFontLoader.cc" 
                    INCLUDE_DIRS "include"
                    REQUIRES )
component_compile_options(-std=gnu++17)
