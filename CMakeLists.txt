cmake_minimum_required( VERSION 2.8 )
project( SkeletonRecord )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/" ${CMAKE_MODULE_PATH} )

FIND_PACKAGE(Threads)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package( OpenNI2 REQUIRED )
include_directories( ${OPENNI2_INCLUDE_DIRS} )

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM REQUIRED gtkmm-3.0)
INCLUDE_DIRECTORIES(${GTKMM_INCLUDE_DIRS})

set(NITE2_INCLUDE_DIRS ~/NiTE-Linux-x64-2.2/Include )
set(NITE2_LIBS ~/NiTE-Linux-x64-2.2/Redist/libNiTE2.so)
include_directories( ${NITE2_INCLUDE_DIRS})

include_directories( ${CMAKE_SOURCE_DIR}/include )

add_executable( Record
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/SkelRecorder.cpp
)

target_link_libraries( Record 
    ${OpenCV_LIBS} 
    ${OPENNI2_LIBRARIES}
    ${NITE2_LIBS}
    ${GTKMM_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)

add_custom_command(TARGET Record PRE_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_SOURCE_DIR}/src/mainwindow.glade $<TARGET_FILE_DIR:Record>)
