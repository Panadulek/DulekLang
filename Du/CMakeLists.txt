# Znajdź i dodaj Bison i Flex
project ("Du")

set(3RD_PARTY_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/../include_3rd_party")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# Ustaw pliki wejściowe dla Bisona i Flexa
set(FLEX_INPUT lexer/scanner.l)
set(BISON_INPUT parser/parser.y)
set(LIBS_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/libs)


set(BISON_FLAGS "-v")
set(FLEX_FLAGS "")  

# Wygeneruj parser i skaner
BISON_TARGET(Parser ${BISON_INPUT} ${CMAKE_CURRENT_SOURCE_DIR}/gen/parser.cpp)
FLEX_TARGET(Scanner ${FLEX_INPUT} ${CMAKE_CURRENT_SOURCE_DIR}/gen/scanner.cpp)

# Dodaj zależność między parserem a skanerem
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

#utworzenie statycznej biblioteki ast
add_library(DuAst STATIC
    ast/AstElement.cpp
    ast/AstScope.cpp
    ast/AstVariableDecl.cpp
    ast/AstCast.cpp
    ast/AstUniqueCast.cpp
    ast/AstFactor.cpp 
    ast/AstStatement.cpp
    ast/AstCallFun.cpp
    ast/ScopeDecorator.cpp
    ast/AstVariableDecl.cpp
    Terminal/StandardTerminal.cpp
    Terminal/Terminal.cpp 
    ast/AstType.cpp 
    ast/AstExpr.cpp
    ast/VariableDecorator.cpp)

# Dodaj wykonywalny cel projektu
add_executable (Du
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Scanner_OUTPUTS}
    "Program/main.cpp" 
    "Program/Settings.h"
    "Program/Parser.cpp"
  "llvm_gen/llvm_generator.h" "llvm_gen/llvm_output_process.h" "llvm_gen/llvm_type_generator.hpp"  "llvm_gen/llvm_generator.cpp" "llvm_gen/llvm_stmt_generator.hpp" "llvm_gen/llvm_cache.hpp" "Terminal/StandardTerminal.cpp" "Terminal/Terminal.cpp")


target_include_directories(Du PRIVATE ${3RD_PARTY_INCLUDES}/gnu)
target_link_libraries(Du PRIVATE DuAst)
set_target_properties(DuAst PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
)
# Ustaw wersję standardu języka
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET DuAst PROPERTY CXX_STANDARD 20)
    set_property(TARGET Du PROPERTY CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

add_definitions(-DNOMINMAX)
target_compile_definitions(Du PRIVATE NOMINMAX)



set(LLVM_DIR ${3RD_PARTY_INCLUDES}/llvm)
set(LLVM_INCLUDE_DIR ${LLVM_DIR}/include)
set(LLVM_LIB_DIR_DEBUG ${LLVM_DIR}/lib/debug)



include_directories(${LLVM_INCLUDE_DIR})
link_directories(${LLVM_LIB_DIR_DEBUG})
file(GLOB LLVM_LIB_FILE ${LLVM_LIB_DIR_DEBUG}/*.lib)
target_link_libraries(Du PRIVATE ${LLVM_LIB_FILE} ws2_32)