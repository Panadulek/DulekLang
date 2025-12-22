cmake_minimum_required(VERSION 3.20)

# =============================================================
# 1. PROJEKT I STANDARDY (C++20)
# =============================================================
project("Du" CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Porządkowanie folderów w Visual Studio (Solution Explorer)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Wymuszenie Dynamicznego Runtime (MDd), aby pasował do LLVM z Conana
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# Ścieżki projektowe
set(CONAN_LOCAL_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include_3rd_party/conan_deps")
set(3RD_PARTY_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/../include_3rd_party")

# =============================================================
# 2. KONFIGURACJA REPOZYTORIUM CONANA (DYSK E:, VS 18 INSIDERS)
# =============================================================
set(MY_CONAN_HOME "E:/tools/conan_cache")
file(TO_NATIVE_PATH "${MY_CONAN_HOME}" CONAN_HOME_NATIVE)
set(ENV{CONAN_HOME} "${CONAN_HOME_NATIVE}")

set(MY_VS_PATH "E:/Program Files/Microsoft Visual Studio/18/Insiders")
file(TO_NATIVE_PATH "${MY_VS_PATH}" MY_VS_PATH_NATIVE)

# =============================================================
# 3. AUTOMATYZACJA / INSTALACJA LLVM PRZEZ CONANA
# =============================================================
find_package(Python3 REQUIRED COMPONENTS Interpreter)

function(setup_conan_and_llvm)
    if(EXISTS "${CONAN_LOCAL_INSTALL_DIR}/LLVMConfig.cmake" OR EXISTS "${CONAN_LOCAL_INSTALL_DIR}/llvm-core-config.cmake")
        set(CMAKE_PREFIX_PATH "${CONAN_LOCAL_INSTALL_DIR}" PARENT_SCOPE)
        return()
    elseif(EXISTS "${CONAN_LOCAL_INSTALL_DIR}/build/generators/LLVMConfig.cmake")
        set(CMAKE_PREFIX_PATH "${CONAN_LOCAL_INSTALL_DIR}/build/generators" PARENT_SCOPE)
        return()
    endif()

    execute_process(
        COMMAND ${Python3_EXECUTABLE} -m conans.conan install 
                --requires=llvm-core/19.1.7 
                --build=missing 
                --output-folder=${CONAN_LOCAL_INSTALL_DIR}
                --deployer=full_deploy
                -g CMakeDeps
                -s build_type=Debug
                -s compiler.cppstd=20
                -s compiler.runtime=dynamic
                -c "tools.microsoft.msbuild:vs_version=17"
                -c "tools.microsoft.msbuild:installation_path=${MY_VS_PATH_NATIVE}"
                -c "tools.cmake.cmaketoolchain:generator=Ninja"
        RESULT_VARIABLE conan_res
    )

    if(NOT conan_res EQUAL 0)
        message(FATAL_ERROR "Błąd instalacji LLVM przez Conan.")
    endif()

    set(CMAKE_PREFIX_PATH "${CONAN_LOCAL_INSTALL_DIR}" PARENT_SCOPE)
endfunction()

setup_conan_and_llvm()

# =============================================================
# 4. KONFIGURACJA PAKIETÓW (LLVM, BISON, FLEX)
# =============================================================

find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(LLVM_LIBS all)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# Pliki generowane trafiają do BINARY_DIR (folder 'out')
set(GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/gen")
file(MAKE_DIRECTORY "${GEN_DIR}")

# --- FIX: NORMALIZACJA ŚCIEŻKI DLA BISON ---
# Upewniamy się, że ścieżka do mapowania używa forward slashy (/)
# i kończy się slashem, aby "wyciąć" całą ścieżkę do folderu Du/
set(MAP_FROM "${CMAKE_CURRENT_SOURCE_DIR}/")

# Bison 3.7+ mapuje ścieżki absolutne na relatywne w dyrektywach #line
set(BISON_MAP_FLAGS "--file-prefix-map=${MAP_FROM}=")

BISON_TARGET(Parser 
    "${CMAKE_CURRENT_SOURCE_DIR}/parser/parser.y" 
    "${GEN_DIR}/parser.cpp" 
    DEFINES_FILE "${GEN_DIR}/parser.hpp"
    COMPILE_FLAGS "${BISON_MAP_FLAGS}"
)

# Dla Flexa stosujemy podobną flagę (choć Flex rzadziej jej potrzebuje 
# przy użyciu krótkich nazw wyjściowych)
FLEX_TARGET(Scanner 
    "lexer/scanner.l" 
    "${GEN_DIR}/scanner.cpp"
)

ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

# =============================================================
# 5. FLAGI KOMPILACJI (MSVC / CLANG-CL)
# =============================================================
if(MSVC)
    add_compile_definitions(
        -DNOMINMAX
        -D_CRT_SECURE_NO_WARNINGS
        -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
        $<$<CONFIG:Debug>:_DEBUG>
    )
    add_compile_options(/permissive- /Zc:__cplusplus /EHsc)
endif()

# =============================================================
# 6. DEFINICJA TARGETÓW (KOMPILATOR I AST)
# =============================================================

add_library(DuAst STATIC
    Terminal/StandardTerminal.cpp
    Terminal/Terminal.cpp
    ast/AstElement.cpp
    ast/AstScope.cpp
    ast/AstVariableDecl.cpp
    ast/AstCast.cpp
    ast/AstUniqueCast.cpp
    ast/AstFactor.cpp
    ast/AstStatement.cpp
    ast/AstCallFun.cpp
    ast/ScopeDecorator.cpp
    ast/AstType.cpp
    ast/AstExpr.cpp
    ast/VariableDecorator.cpp
)

add_executable(Du
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Scanner_OUTPUTS}
    "Program/main.cpp"
    "Program/Settings.h"
    "Program/Parser.cpp"
    "llvm_gen/llvm_generator.cpp"
)

# =============================================================
# 7. LINKOWANIE I ŚCIEŻKI INCLUDE
# =============================================================

target_link_libraries(Du PRIVATE 
    DuAst 
    ${LLVM_LIBS}
    ws2_32 version dbghelp
)

target_include_directories(Du PRIVATE 
    "${GEN_DIR}"
    "${3RD_PARTY_INCLUDES}/gnu"
    "${CMAKE_CURRENT_SOURCE_DIR}/semantic_analyzer"
    "${LLVM_INCLUDE_DIRS}"
)

target_include_directories(DuAst PRIVATE 
    "${GEN_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/semantic_analyzer"
    "${LLVM_INCLUDE_DIRS}"
)

set_target_properties(Du PROPERTIES FOLDER "DuEngine")
set_target_properties(DuAst PROPERTIES FOLDER "DuEngine")