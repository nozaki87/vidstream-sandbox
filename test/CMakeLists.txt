# test
project(eyetracker_test)
cmake_minimum_required(VERSION 2.6)

include(FindPkgConfig)

#include_directories(
#  ${eyetracker_SOURCE_DIR}/include
#  ${eyetracker_SOURCE_DIR}/src
#)

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

find_package(OpenCV REQUIRED)

if(OpenCV_FOUND)
  message(STATUS "Found library: OpenCV")
  include_directories(${OpenCV_INCLUDE_DIRS})
  link_libraries(${OpenCV_LIBS})
endif(OpenCV_FOUND)

add_executable(sender01 src/sender01.cpp src/udp_wrapper.cpp)
target_link_libraries(sender01 ${local_lib})
add_executable(receiver01 src/receiver01 src/udp_wrapper.cpp)
target_link_libraries(receiver01 ${local_lib})
