cmake_minimum_required( VERSION 3.0 )

project( run_tests )

include_directories( ${ROOT_DIR}/projects/rectangles_intersections/sources )

set( HEADER ${ROOT_DIR}/projects/rectangles_intersections/sources/rects.h
            )

set( SOURCES sources/main.cpp
             ${ROOT_DIR}/projects/rectangles_intersections/sources/rects.cpp
             )

add_executable( run_tests ${HEADER} ${SOURCES} )