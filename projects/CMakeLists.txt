add_subdirectory( rectangles_intersections )

if(BUILD_TOOLS)
  add_subdirectory( generate_test )
  add_subdirectory( run_tests )
endif(BUILD_TOOLS)