cmake_minimum_required(VERSION 3.0.0)
project(simple-zlib VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(simple-zlib simple-zlib.cpp lib.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
