cmake_minimum_required( VERSION 2.8 )
project( SkeletonRecord )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}" )
set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/" ${CMAKE_MODULE_PATH} )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package( OpenNI2 REQUIRED )
include_directories( ${OPENNI2_INCLUDE_DIRS} )

set(NITE2_INCLUDE_DIRS ~/NiTE-Linux-x64-2.2/Include )
set(NITE2_LIBS ~/NiTE-Linux-x64-2.2/Redist/libNiTE2.so)
include_directories( ${NITE2_INCLUDE_DIRS})

add_executable( Record
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/SkelRecorder.cpp
    ${CMAKE_SOURCE_DIR}/mainwindow.ui
    ${CMAKE_SOURCE_DIR}/mainwindow.cpp
)

target_link_libraries( Record 
    ${OpenCV_LIBS} 
    ${OPENNI2_LIBRARIES}
    ${NITE2_LIBS}
    Qt5::Widgets
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)
