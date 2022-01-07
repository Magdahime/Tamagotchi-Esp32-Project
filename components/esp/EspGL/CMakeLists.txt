idf_component_register(SRCS "EspGLUtils.cc" "Graphics/Font/EspGLFontLoader.cc" "Graphics/Font/EspGLFont.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES BitmapLoader SPIFFSDriver)
component_compile_options(-std=gnu++17)
