cmake_minimum_required(VERSION 3.8)
project(RSAImpl CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -static-libgcc -static-libstdc++")

set(SOURCE_FILES
        src/main.cpp
        src/long_arithm.h
        src/long_arithm.hpp
        src/random.h
        src/random.hpp
        src/rsa_encryptor.h
        src/rsa_encryptor.hpp
)

add_executable(RSAImpl ${SOURCE_FILES})
