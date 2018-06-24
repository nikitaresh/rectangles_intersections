This repository contains an implementation of rectangles intersections algorithm.

## Problem description
The task is to find all intersections involving two or more rectangles.

An algorithm complexity of naive implementation is equal to O(N*2<sup>N</sup>). In the worst case, N identical rectangles, the answer is 2<sup>N</sup>-N-1 intersections.

In sparse case, when number of intersections doesn't grow exponentially, the number of comparisons can be greatly reduced. The proposed method is based on a known [Interval Tree](https://en.wikipedia.org/wiki/Interval_tree) structure. The implementation of Interval Tree was taken from [this](https://github.com/IvanPinezhaninov/IntervalTree) repository.

Interval tree is used to store 'y' intervals of rectangles. Each rectangle is compared only with rectangles which intervals are overlapping. In this case, algorithm complexity is close to O(N log(2<sup>N</sup>)), that is O(N<sup>2</sup>).

## Requirements
This project was written in Cross-Platform C++ and depends only on CMake. The minimum requirement is CMake 3.0. All other libraries are already located in [thirdparty](thirdparty/) directory.
Minimal requirements for

*Windows*:
    Visual Studio 14 2015

*Linux*:
    GCC 4.9

## Build steps
1. Generate a project using CMake. Linux commands:

       cd build
       cmake ..

2. Build the project. Linux command:

       make

## Run tests
The [data](data/) directory contains tests and a [tests_list](data/tests_list.txt) file where listed all tests for batch testing. Before start a [run_tests](projects\run_tests) application, make sure that all applications located in directory bin.release or bin.debug. Run test in Linux:

    cd bin.release
    ./run_tests ../data/run_tests.txt
