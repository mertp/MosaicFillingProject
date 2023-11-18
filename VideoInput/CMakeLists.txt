cmake_minimum_required(VERSION 2.8)
project(VideoInput)

# Set the OpenCV_DIR to the directory where OpenCVConfig.cmake is located.
set(OpenCV_DIR /home/mertp/Documents/build)  # Update the path if necessary

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(VideoInput VideoInput.cpp)
target_link_libraries(VideoInput ${OpenCV_LIBS})
