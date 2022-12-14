cmake_minimum_required(VERSION 3.16)
project(tests)

SET (CMAKE_BUILD_TYPE "Release")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17")
SET (CMAKE_CXX_FLAGS_RELEASE "-O3")
SET (CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address")

SET (BASE_DIR ${CMAKE_SOURCE_DIR} )
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/../tests-bin) 

## Exercise 1
add_executable( testFCM testFCM.cpp )

## Exercise 2
add_executable( lang lang.cpp )

## Exercise 3
add_executable( findLang findLang.cpp )
