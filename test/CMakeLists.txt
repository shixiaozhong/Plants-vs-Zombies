cmake_minimum_required(VERSION 3.20)
project(Test)

aux_source_directory("../src" SRCS) # 获取src下的所有cpp文件放到SRCS中
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}" TEST_SRCS)   # 获取test目录下的所有cpp文件放到TEST_SRCS中

# 如果存在多个test文件，使用foreach
add_executable(coreminial_test ${CMAKE_CURRENT_SOURCE_DIR}/coreminial_test.cpp)

target_link_directories(coreminial_test PUBLIC ${CMAKE_SOURCE_DIR}/third_party/googletest-1.14.0/googletest/include)
target_link_directories(coreminial_test PUBLIC ../src/include)

target_link_libraries(coreminial_test
    PRIVATE
    gtest
    gtest_main
)

# ctest
add_test(NAME coreminial_test COMMAND ${PROJECT_NAME})

# add_dependencies(coreminial_test gtest gtest_main)