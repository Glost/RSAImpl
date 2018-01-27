cmake_minimum_required(VERSION 3.8)
project(RSAImpl)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.cpp)
add_executable(RSAImpl ${SOURCE_FILES})
