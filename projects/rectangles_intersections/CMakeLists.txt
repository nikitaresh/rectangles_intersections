cmake_minimum_required( VERSION 3.0 )

project( rectangles_intersections )

include_directories( sources )
include_directories( ${ROOT_DIR}/thirdparty/IntervalTree/include )

set( HEADER sources/rects.h
            sources/rectangles_intersections.h
            ${ROOT_DIR}/thirdparty/IntervalTree/include/intervaltree.hpp
            )

set( SOURCES sources/rects.cpp
             sources/rectangles_intersections.cpp
             )

add_library( rectangles_intersections ${HEADER} ${SOURCES} )
target_link_libraries( rectangles_intersections )