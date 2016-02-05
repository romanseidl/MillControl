cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)


set(PROJECT_NAME MillControl)
project(${PROJECT_NAME})

link_directories(/home/roman/sketchbook/libraries/U8glib)
set(U8glib_RECURSE True)
link_directories(/home/roman/sketchbook/libraries/Queuetue_HX711_Library/src)
#set(U8glib_RECURSE True)
link_directories(/home/roman/sketchbook/libraries/)


set(${CMAKE_PROJECT_NAME}_SKETCH MillControl.ino)

set(${CMAKE_PROJECT_NAME}_BOARD pro5v328)
set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyUSB0)
set(${CMAKE_PROJECT_NAME}_SERIAL picocom @SERIAL_PORT@ -b 9600 -l)


generate_arduino_firmware(${CMAKE_PROJECT_NAME})
