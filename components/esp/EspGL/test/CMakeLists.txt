idf_component_register(SRCS "testEspGLUtils.cc" "testEspGLShapes.cc" "testEspGLFontLoader.cc"
                       INCLUDE_DIRS "include"
                       REQUIRES unity test_utils ST7789Driver EspGL SPIFFSDriver)