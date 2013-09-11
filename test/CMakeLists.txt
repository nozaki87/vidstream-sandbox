# test
project(eyetracker_test)
cmake_minimum_required(VERSION 2.6)

include(FindPkgConfig)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/files.cmake/modules")

#include_directories(
#  ${eyetracker_SOURCE_DIR}/include
#  ${eyetracker_SOURCE_DIR}/src
#)

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

find_package(OpenCV REQUIRED)
find_package(LIBJPEG REQUIRED)

if(OpenCV_FOUND)
  message(STATUS "Found library: OpenCV")
  include_directories(${OpenCV_INCLUDE_DIRS})
  link_libraries(${OpenCV_LIBS})
endif(OpenCV_FOUND)
if(LIBJPEG_FOUND)
  message(STATUS "Found library: libjpeg")
  include_directories(${LIBJPEG_INCLUDE_DIRS})
  link_libraries(${LIBJPEG_LIBRARIES})
endif(LIBJPEG_FOUND)

set(v4l2src src/v4l2w.c src/jpeghelper.c)  

add_executable(sender01 src/sender01.cpp src/udp_wrapper.cpp)
target_link_libraries(sender01 ${local_lib})
add_executable(sender02 src/sender02.cpp src/ndp.cpp src/udp_wrapper.cpp)
target_link_libraries(sender02 ${local_lib})
add_executable(sender03 src/sender03.cpp ${v4l2src})
target_link_libraries(sender03 ${local_lib})
add_executable(receiver01 src/receiver01 src/udp_wrapper.cpp)
target_link_libraries(receiver01 ${local_lib})
add_executable(receiver02 src/receiver02 src/ndp.cpp src/udp_wrapper.cpp)
target_link_libraries(receiver02 ${local_lib})
