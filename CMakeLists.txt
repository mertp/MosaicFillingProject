cmake_minimum_required(VERSION 2.8)
project(MosaicFillingProject)

# Set the OpenCV_DIR to the directory where OpenCVConfig.cmake is located.
set(OpenCV_DIR /home/mertp/Documents/build)  # Update the path if necessary

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable (MosaicFillingProject "MosaicFillingProject.cpp" "MosaicFillingProject.h"  "birdview.cpp" "colordetection.cpp" "birdview.h" "colordetection.h")
target_include_directories(MosaicFillingProject PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(MosaicFillingProject ${OpenCV_LIBS})
