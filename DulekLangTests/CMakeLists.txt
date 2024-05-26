project ("DulekLangTests")
set(3RD_PARTY_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/../include_3rd_party")
set(GTEST_DIR "${3RD_PARTY_INCLUDES}/googletest")
set(GTEST_DIR_INCLUDE "${GTEST_DIR}/src/googletest/googletest/include")
#set(GTEST_DIR_LIB "${GTEST_DIR}/src/googletest-build/lib")
set(GTEST_DIR_LIB "${GTEST_DIR}/src/googletest-build/lib")
include(ExternalProject)
ExternalProject_Add(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
    PREFIX ${GTEST_DIR}
        CMAKE_ARGS 
        -DCMAKE_INSTALL_PREFIX=${GTEST_INSTALL_DIR}
        -DCMAKE_CXX_STANDARD=20
        -DCMAKE_CXX_STANDARD_REQUIRED=ON
        -DCMAKE_BUILD_TYPE=Release
)


#set(PROJECT_DIR "E:/LocalRepo/Compilers/DulekLang/DulekLang/Program")
set(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Du")


add_executable(DulekLangTests
    "${PROJECT_DIR}/Program/Settings.h"
    
    "main.cpp")


target_include_directories(DulekLangTests PRIVATE ${GTEST_DIR_INCLUDE})
if (WIN32)
    message("DEBUG:${GTEST_DIR_LIB}")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")

        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")        
        target_link_libraries(DulekLangTests PRIVATE "${GTEST_DIR_LIB}/gtestd.lib" "${GTEST_DIR_LIB}/gtest_maind.lib" "${GTEST_DIR_LIB}/gmockd.lib" "${GTEST_DIR_LIB}/gmock_maind.lib")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD")

        target_link_libraries(DulekLangTests PRIVATE
            "${GTEST_DIR_LIB}/gtestd.lib" "${GTEST_DIR_LIB}/gtest_maind.lib" "${GTEST_DIR_LIB}/gmockd.lib" "${GTEST_DIR_LIB}/gmock_maind.lib"
        )
    endif()
endif()
add_dependencies(DulekLangTests googletest)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DulekLangTests PROPERTY CXX_STANDARD 20)
endif()


