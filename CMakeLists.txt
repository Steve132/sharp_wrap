cmake_minimum_required(VERSION 2.8)
project(sharp_wrap)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

set(SHARP_INCLUDE_DIR "" CACHE PATH "The libsharp include directory")
set(SHARP_LIBRARY_DIR "" CACHE PATH "The libsharp lib directory")

link_directories(${SHARP_LIBRARY_DIR})
include_directories(${SHARP_INCLUDE_DIR})

add_library(sharp_wrap_latlong sharp_latlong.cpp sharp_latlong.hpp)
target_link_libraries(sharp_wrap_latlong sharp c_utils fftpack)

add_executable(sharp_wrap_test sharp_wrap_test.cpp)
target_link_libraries(sharp_wrap_test sharp_wrap_latlong)
