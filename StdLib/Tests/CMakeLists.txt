
project(StdLibTests)
find_package(GTest REQUIRED CONFIG)
add_executable(StdLibTests main.cpp "memory_menagment_test.h")
target_link_libraries(StdLibTests PRIVATE GTest::gtest GTest::gtest_main)
target_link_libraries(StdLibTests PRIVATE StdLib)
include(GoogleTest)
gtest_discover_tests(StdLibTests)
