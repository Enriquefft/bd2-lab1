cmake_minimum_required(VERSION 3.26)

project(
  BD2_Lab1
  VERSION 1.0
  DESCRIPTION "Lab 1 for BD2"
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(common INTERFACE)

target_compile_features(common INTERFACE cxx_std_20)

target_compile_options(
  common
  INTERFACE
    # Prefered warnings
    $<$<CXX_COMPILER_ID:MSVC>:/W4
    /WX>
    $<$<CXX_COMPILER_ID:Clang>:-Weverything>
    $<$<CXX_COMPILER_ID:GNU>:-Wall
    -Wextra
    -Wpedantic
    -Werror
    -fanalyzer
    >
    # Warning format
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fdiagnostics-show-option>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fdiagnostics-color=always>
    # Address sanitizer
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize-address-use-after-scope>
    # Optimizations
    $<$<CONFIG:RELEASE>:-Ofast>
    $<$<CONFIG:DEBUG>:-O0>
    $<$<CONFIG:DEBUG>:-ggdb3>
    # Disable some warnings
    $<$<CXX_COMPILER_ID:Clang>:-Wno-c++98-compat>
    $<$<CXX_COMPILER_ID:Clang>:-Wno-c++98-compat-pedantic>
    $<$<CXX_COMPILER_ID:Clang>:-Wno-c++20-compat>
    $<$<CXX_COMPILER_ID:Clang>:-Wno-zero-as-null-pointer-constant> # Fixes bug?
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-error=padded>)

target_link_options(
  common INTERFACE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined>)

add_subdirectory("P1")

target_link_libraries(p1 PRIVATE common)
# target_link_libraries(p2 PRIVATE common) target_link_libraries(p3 PRIVATE
# common)
