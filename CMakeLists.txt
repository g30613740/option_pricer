cmake_minimum_required(VERSION 3.20)
project(option_pricer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

option(OPTION_PRICER_BUILD_TESTS "Build unit tests" ON)
option(OPTION_PRICER_BUILD_BENCH "Build benchmarks" ON)

include(FetchContent)

# Core library
add_library(option_pricer src/math.cpp)
target_include_directories(option_pricer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(option_pricer PRIVATE -Wall -Wextra -Wpedantic)

# Command-line application
add_executable(option_pricer_app app/main.cpp)
target_link_libraries(option_pricer_app PRIVATE option_pricer)

if(OPTION_PRICER_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(OPTION_PRICER_BUILD_BENCH)
  add_subdirectory(bench)
endif()
