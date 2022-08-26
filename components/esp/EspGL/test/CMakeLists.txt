idf_component_register(SRCS "testEspGLUtils.cc" "testEspGLShapes.cc" "testEspGLFontLoader.cc"
                           "testEspGLText.cc" "initializeDisplay.cc" "testEspGLBitmap.cc"
                           "testPerformance.cc"
                       INCLUDE_DIRS "include"
                       REQUIRES unity test_utils ST7789Driver EspGL Globals BitmapLoader)
component_compile_options(-std=gnu++17)