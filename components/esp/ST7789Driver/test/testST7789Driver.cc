// TEST(ST7789VWDriverTests, DrawingCircle) {
//   driver.drawCircle({100, 100}, 50, ST7789::colours::YELLOW);
// }

// TEST(ST7789VWDriverTests, DrawingFilledCircle) {
//   driver.drawFilledCircle({100, 100}, 50, ST7789::colours::YELLOW);
// }

// TEST(ST7789VWDriverTests, DrawingPolygon) {
//   driver.drawPolygon({100, 100}, 50, 4, ST7789::colours::YELLOW);
// }

// TEST(ST7789VWDriverTests, DrawingRotatedPolygon) {
//   driver.drawPolygon({100, 100}, 50, 4, ST7789::colours::YELLOW,
//                      ST7789::consts::PI / 4.0);
// }

// TEST(ST7789VWDriverTests, DrawingRotatedPolygonWithManyVertices) {
//   driver.drawPolygon({100, 100}, 50, 16, ST7789::colours::YELLOW,
//                      ST7789::consts::PI / 4.0);
// }

// TEST(ST7789VWDriverTests, DrawingFilledPolygon) {
//   driver.drawFilledPolygon({100, 100}, 50, 4, ST7789::colours::YELLOW);
// }

// TEST(ST7789VWDriverTests, DrawingFilledRotatedPolygon) {
//   driver.drawFilledPolygon({100, 100}, 50, 4, ST7789::colours::YELLOW,
//                            ST7789::consts::PI / 4.0);
// }

// TEST(ST7789VWDriverTests, DrawingFilledRotatedPolygonWithManyVertices) {
//   driver.drawFilledPolygon({100, 100}, 50, 16, ST7789::colours::YELLOW,
//                            ST7789::consts::PI / 4.0);
// }

// TEST(ST7789VWDriverTests, DrawingBitmap) {
//   std::vector<bool> test{
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, true,  true,  true,  true,
//       true,  true,  false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, true,  false, false, false, false,
//       false, false, true,  false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, true,  false, false, false, false,
//       false, false, true,  false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, true,  false, false, false, false,
//       false, false, true,  false, false, false, false, false, false, false,
//       false, true,  true,  true,  true,  true,  false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, true,  false, false, false, false,
//       false, false, true,  false, false, false, false, false, false, false,
//       false, false, false, false, false, false, true,  false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, true,  true,  true,  true,
//       true,  true,  false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, true,  false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, true,  false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, true,  false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, true,  false, false, false, false, false, false,
//       false, false, false, false, false, false, true,  true,  false, false,
//       false, false, false, false, false, false, false, false, false, true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, true,  false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, true,  false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, true,  false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, true,  true,  false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, true,  true,  false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, true,  true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, true,  true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       true,  true,  true,  false, false, false, false, false, false, false,
//       true,  true,  true,  false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, true,  true,  true,  true,  true,  true,  true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, true,  true,  true,  false, false,
//       false, true,  true,  true,  true,  false, false, false, false, true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, true,  false, false, false, true,  false,
//       false, true,  false, false, false, true,  false, false, false, true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, true,  false, false, false, false, false, true,
//       false, true,  false, false, false, false, true,  false, false, true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, true,  false, false, false, false, false, false,
//       false, true,  false, false, false, false, false, true,  false, true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, true,  true,  false, false, false, false,
//       false, true,  false, false, false, false, false, true,  false, true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, true,  false, false, false,
//       false, true,  false, false, false, false, false, true,  false, true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, true,  true,  false,
//       false, true,  false, false, false, false, false, true,  false, true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, true,  false,
//       false, true,  false, false, false, false, false, true,  false, true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, true,  false, false, false, false, false, true,  false,
//       false, true,  false, false, false, false, true,  false, false, true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, true,  false, false, true,  true,  false, false,
//       false, true,  false, false, false, true,  false, false, false, true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, true,  false, true,  false, false, false,
//       false, true,  true,  true,  true,  false, false, false, false, true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, true,  true,  true,  true,  false, false, false, false,
//       false, true,  true,  true,  true,  true,  false, false, false, false,
//       false, true,  true,  true,  true,  false, false, false, false, false,
//       false, false, false, false, true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, true,  false, false, false, false, true,  false, false, false,
//       true,  false, false, false, false, false, true,  false, false, false,
//       true,  false, false, false, false, true,  false, false, false, false,
//       false, false, false, true,  true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       true,  false, false, false, false, false, false, true,  false, false,
//       true,  false, false, false, false, false, true,  false, false, true,
//       false, false, false, false, false, false, true,  false, false, false,
//       false, false, true,  false, true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, true,  false, false,
//       true,  false, false, false, false, false, true,  false, false, false,
//       false, false, false, false, false, false, true,  false, false, false,
//       false, true,  false, false, true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, true,  false, false,
//       true,  false, false, false, false, false, true,  false, false, false,
//       false, false, false, false, false, false, true,  false, false, false,
//       true,  false, false, false, true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, true,  false, false, false,
//       true,  false, false, false, false, false, true,  false, false, false,
//       false, false, false, false, false, true,  false, false, false, true,
//       false, false, false, false, true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, true,  false, false, false, false,
//       true,  false, false, false, false, false, true,  false, false, false,
//       false, false, false, false, true,  false, false, false, false, false,
//       false, false, false, false, true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, true,  false, false, false, false, false,
//       true,  false, false, false, false, false, true,  false, false, false,
//       false, false, false, true,  false, false, false, false, false, false,
//       false, false, false, false, true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, true,  false, false, false, false, false, false,
//       true,  false, false, false, false, false, true,  false, false, false,
//       false, false, true,  false, false, false, false, false, false, false,
//       false, false, false, false, true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, true,  false, false, false, false, false, false, false,
//       true,  false, false, false, false, false, true,  false, false, false,
//       false, true,  false, false, false, false, false, false, false, false,
//       false, false, false, false, true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, true,  false, false, false, false, false, false, false, false,
//       true,  false, false, false, false, false, true,  false, false, false,
//       true,  false, false, false, false, false, false, false, false, false,
//       false, false, false, false, true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       true,  false, false, false, false, false, false, false, false, false,
//       true,  false, false, false, false, false, true,  false, false, true,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, true,
//       true,  true,  true,  true,  true,  true,  true,  true,  true,  false,
//       false, true,  true,  true,  true,  true,  false, false, true,  true,
//       true,  true,  true,  true,  true,  true,  true,  true,  false, false,
//       false, false, false, false, true,  false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,
//       false, false, false, false, false, false, false, false, false, false,

//   };
//   driver.drawBitmap({100, 100}, 50, 50, ST7789::colours::RED, test);
// }


// TEST(ST7789VWDriverTests, DrawingEllipse) {
//   driver.drawEllipse({100, 100}, 15, 10, ST7789::colours::YELLOW);
// }


// TEST(ST7789VWDriverTests, DrawingFilledEllipse) {
//   driver.drawFilledEllipse({100, 100}, 15, 10, ST7789::colours::YELLOW);
// }

// TEST_GROUP_RUNNER(ST7789VWDriverTests) {
//   // RUN_TEST_CASE(ST7789VWDriverTests, FillingDisplayTest)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingPixelTest)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingPixelLine)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingFilledRectangle)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingFilledRectangles)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingHorizontalLine)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingVerticalLine)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingLines)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingRectangleOutline)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingTriangle)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingFilledTriangle)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingCircle)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingFilledCircle)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingPolygon)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingRotatedPolygon)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingRotatedPolygonWithManyVertices)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingFilledPolygon)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingFilledRotatedPolygon)
//   // RUN_TEST_CASE(ST7789VWDriverTests,
//   //               DrawingFilledRotatedPolygonWithManyVertices)
//   // RUN_TEST_CASE(ST7789VWDriverTests, DrawingBitmap)
//   RUN_TEST_CASE(ST7789VWDriverTests, DrawingEllipse)
//   RUN_TEST_CASE(ST7789VWDriverTests, DrawingFilledEllipse)
// }