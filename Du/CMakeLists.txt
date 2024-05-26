# Znajdź i dodaj Bison i Flex
project ("Du")
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# Ustaw pliki wejściowe dla Bisona i Flexa
set(FLEX_INPUT lexer/scanner.l)
set(BISON_INPUT parser/parser.y)

# Wygeneruj parser i skaner
BISON_TARGET(Parser ${BISON_INPUT} ${CMAKE_CURRENT_SOURCE_DIR}/gen/parser.cpp)
FLEX_TARGET(Scanner ${FLEX_INPUT} ${CMAKE_CURRENT_SOURCE_DIR}/gen/scanner.cpp)

# Dodaj zależność między parserem a skanerem
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

# Dodaj wykonywalny cel projektu
add_executable (Du
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Scanner_OUTPUTS}
    "Program/main.cpp" 
    "Program/Settings.h"
     "Program/Parser.hpp" "Program/Parser.cpp" "ast/AstElement.cpp" "ast/AstScope.cpp")

set(3RD_PARTY_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/../include_3rd_party")
target_include_directories(Du PRIVATE ${3RD_PARTY_INCLUDES}/gnu)

# Ustaw wersję standardu języka
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET Du PROPERTY CXX_STANDARD 20)
    if (WIN32)
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")        
        else()
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
            set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD")
        endif()
    endif()
endif()

