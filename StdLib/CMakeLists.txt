project(StdLib)

add_library(StdLib STATIC
    MemoryMenagment/memory_menagment.cpp
)

target_include_directories(StdLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Tests)
