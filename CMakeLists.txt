cmake_minimum_required(VERSION 3.8)
project(RSAImpl CXX)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.cpp
        src/long_arithm.h
        src/long_arithm.hpp
        src/random.h
        src/random.hpp
)

add_executable(RSAImpl ${SOURCE_FILES})
