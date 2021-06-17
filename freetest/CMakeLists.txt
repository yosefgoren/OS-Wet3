cmake_minimum_required(VERSION 3.0.0)
project(freetest VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(freetest main.c ../webserver-files/Queue.c)
add_executable(queuetest quick.c ../webserver-files/Queue.c)
set(CMAKE_C_FLAGS "-g")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
