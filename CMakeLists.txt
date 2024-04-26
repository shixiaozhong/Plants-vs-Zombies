cmake_minimum_required(VERSION 3.20)

project(Plants-vs-Zombies LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

#ctest
enable_testing()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/googletest-1.14.0)
add_subdirectory(test)