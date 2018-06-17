cmake_minimum_required( VERSION 3.0 )

project( rectangles_intersections )

include_directories( sources )
include_directories( ${ROOT_DIR}/thirdparty/jsoncpp-0.10.6/include )
include_directories( ${ROOT_DIR}/thirdparty/IntervalTree/include )

set( HEADER sources/rectangles_intersections.h
            ${ROOT_DIR}/thirdparty/IntervalTree/include/intervaltree.hpp
            )

set( SOURCES sources/main.cpp
             sources/rectangles_intersections.cpp
             )

add_executable( rectangles_intersections ${HEADER} ${SOURCES} )
target_link_libraries( rectangles_intersections jsoncpp_lib )