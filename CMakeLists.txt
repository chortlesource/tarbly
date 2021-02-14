#### TARBLY CMAKE FILE
cmake_minimum_required(VERSION 3.1)
project(tarbly)

## PROJECT FILES
include_directories(${CMAKE_SOURCE_DIR}/inc)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

## EXECUTABLE
add_executable(${PROJECT_NAME} ${PROJECT_SRC})

## FLAGS
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -DDEBUG_BUILD")
