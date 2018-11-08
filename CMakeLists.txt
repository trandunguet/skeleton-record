cmake_minimum_required( VERSION 2.8 )
project( SkeletonRecord )

set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/" ${CMAKE_MODULE_PATH} )

find_package( OpenCV REQUIRED )
find_package( OpenNI2 REQUIRED )

set(NITE2_INCLUDE_DIRS ~/NiTE-Linux-x64-2.2/Include )
set(NITE2_LIBS ~/NiTE-Linux-x64-2.2/Redist/libNiTE2.so)

include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${OPENNI2_INCLUDE_DIRS} )
include_directories( ${NITE2_INCLUDE_DIRS})

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

include_directories( ${CMAKE_SOURCE_DIR}/include )

add_executable( Record
    src/main.cpp
    src/SkelRecorder.cpp
)

target_link_libraries( Record ${OpenCV_LIBS} OpenNI2 ${NITE2_LIBS} )

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)
