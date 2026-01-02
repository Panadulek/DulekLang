cmake_minimum_required (VERSION 3.12)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# =============================================================
# CONAN CONFIGURATION FOR GTEST
# =============================================================
set(CONAN_LOCAL_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include_3rd_party/conan_deps")
set(MY_CONAN_HOME "E:/tools/conan_cache")
file(TO_NATIVE_PATH "${MY_CONAN_HOME}" CONAN_HOME_NATIVE)
set(ENV{CONAN_HOME} "${CONAN_HOME_NATIVE}")

set(MY_VS_PATH "E:/Program Files/Microsoft Visual Studio/18/Insiders")
file(TO_NATIVE_PATH "${MY_VS_PATH}" MY_VS_PATH_NATIVE)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

function(setup_conan_gtest)
    if(EXISTS "${CONAN_LOCAL_INSTALL_DIR}/GTestConfig.cmake" OR EXISTS "${CONAN_LOCAL_INSTALL_DIR}/gtest-config.cmake")
        set(CMAKE_PREFIX_PATH "${CONAN_LOCAL_INSTALL_DIR}" PARENT_SCOPE)
        return()
    endif()

    execute_process(
        COMMAND ${Python3_EXECUTABLE} -m conans.conan install 
                --requires=gtest/1.11.0
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
    if(NOT "${conan_res}" STREQUAL "0")
        message(FATAL_ERROR "Conan install failed: ${conan_res}")
    endif()
    set(CMAKE_PREFIX_PATH "${CONAN_LOCAL_INSTALL_DIR}" PARENT_SCOPE)
endfunction()

setup_conan_gtest()
list(APPEND CMAKE_PREFIX_PATH "${CONAN_LOCAL_INSTALL_DIR}")

add_subdirectory ("Du")
add_subdirectory ("StdLib")

add_subdirectory("DulekLangTests")