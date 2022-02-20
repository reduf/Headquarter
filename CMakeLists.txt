cmake_minimum_required(VERSION 3.1)

project(
  Headquarter
  VERSION 1.0
  LANGUAGES C
)

option(ENABLE_TESTING "Build tests" OFF)

add_subdirectory(code)
add_subdirectory(deps)

if (ENABLE_TESTING)
  add_subdirectory(tests)
endif()
