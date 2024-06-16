# Znajdź i dodaj Bison i Flex
project ("Du")
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# Ustaw pliki wejściowe dla Bisona i Flexa
set(FLEX_INPUT lexer/scanner.l)
set(BISON_INPUT parser/parser.y)
set(LIBS_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/libs)
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
 "ast/AstBuilder.hpp"  "ast/AstCast.hpp"  "ast/ScopeDecorator.hpp" "ast/BasicType.hpp" )

# Dodaj wykonywalny cel projektu
add_executable (Du
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Scanner_OUTPUTS}
    "Program/main.cpp" 
    "Program/Settings.h"
    "Program/Parser.cpp"
 "ast/AstBuilder.hpp"  "ast/AstCast.hpp"  "ast/ScopeDecorator.hpp" "ast/BasicType.hpp" )

set(3RD_PARTY_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/../include_3rd_party")
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
endif()

