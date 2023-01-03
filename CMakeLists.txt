cmake_minimum_required(VERSION 3.22)
project(merak16_asm)

find_package(Catch2 3 REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.*)
file(GLOB_RECURSE INCLUDE_FILES include/*.*)
file(GLOB_RECURSE TEST_FILES test/*.*)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${INCLUDE_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)
target_include_directories(${PROJECT_NAME} PRIVATE include/)

add_executable(${PROJECT_NAME}_test ${TEST_FILES})
set_property(TARGET ${PROJECT_NAME}_test PROPERTY CXX_STANDARD 14)
target_include_directories(${PROJECT_NAME}_test PRIVATE include/)
target_link_libraries(${PROJECT_NAME}_test PRIVATE Catch2::Catch2WithMain ${PROJECT_NAME})
