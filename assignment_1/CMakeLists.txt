cmake_minimum_required(VERSION 3.16)
project(assignment_1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library
add_library(rmproto
  src\frame.cpp
)

target_include_directories(rmproto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}\include)

target_compile_features(rmproto PUBLIC cxx_std_17)

# CLI
add_executable(rmproto_cli src\main.cpp)

target_link_libraries(rmproto_cli PRIVATE rmproto)

# Tests (GoogleTest via FetchContent)
include(CTest) # defines BUILD_TESTING
if (BUILD_TESTING)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  enable_testing()
  add_executable(rmproto_tests tests\frame_tests.cpp)

  target_link_libraries(rmproto_tests PRIVATE rmproto GTest::gtest_main)

  include(GoogleTest)
  gtest_discover_tests(rmproto_tests)
endif()
