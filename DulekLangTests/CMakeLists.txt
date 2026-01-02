project ("DulekLangTests")

find_package(GTest REQUIRED CONFIG)

set(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Du")

add_executable(DulekLangTests
    "${PROJECT_DIR}/Program/Settings.h"
    "main.cpp" 
    "AstBuilderTest.h")


target_link_libraries(DulekLangTests PRIVATE GTest::gtest GTest::gtest_main DuAst)

target_include_directories(DulekLangTests PRIVATE 
    "${PROJECT_DIR}"
    "${PROJECT_DIR}/include_3rd_party/gnu"

    "${PROJECT_DIR}/ast" 
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DulekLangTests PROPERTY CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

include(GoogleTest)
gtest_discover_tests(DulekLangTests)
