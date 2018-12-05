cmake_minimum_required( VERSION 3.0 )

project( run_tests )

enable_testing()

find_package( GTest REQUIRED )

include_directories( sources )
include_directories( ${ROOT_DIR}/projects/rectangles_intersections/sources )
include_directories( ${ROOT_DIR}/thirdparty/jsoncpp-0.10.6/include )
include_directories( ${ROOT_DIR}/thirdparty/IntervalTree/include )

set( HEADER sources/run_tests_utils.h
            )

set( SOURCES sources/main.cpp
             sources/run_tests_utils.cpp
             )

add_executable( run_tests ${HEADER} ${SOURCES} )
target_link_libraries( run_tests rectangles_intersections jsoncpp_lib GTest::gtest GTest::gtest_main )

add_test( AllTestsInRectanglesIntersections run_tests )