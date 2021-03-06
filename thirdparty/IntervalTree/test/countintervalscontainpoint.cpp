#include "catch.hpp"
#include "intervals.h"

static const int centralPoint = 450;
static const int leftBoundaryPoint = 400;
static const int rightBoundaryPoint = 500;

TEST_CASE("Count intervals contain point")
{
    SECTION("Empty tree")
    {
        const IntervalTree tree;
        const auto count = tree.countIntervalsContainPoint(centralPoint);

        REQUIRE(tree.isEmpty());
        REQUIRE(0 == count);
    }


    SECTION("Boundary interval central point")
    {
        const auto tree = IntervalTree(Test::boundaryIntervals());
        const auto count = tree.countIntervalsContainPoint(centralPoint);

        REQUIRE(count == Test::boundaryIntervals().size());
    }


    SECTION("Boundary interval left boundary point")
    {
        const auto tree = IntervalTree(Test::boundaryIntervals());

        auto count = tree.countIntervalsContainPoint(leftBoundaryPoint, true);
        REQUIRE(count == Test::boundaryIntervals().size());

        count = tree.countIntervalsContainPoint(leftBoundaryPoint, false);
        REQUIRE(0 == count);
    }


    SECTION("Boundary interval right boundary point")
    {
        const auto tree = IntervalTree(Test::boundaryIntervals());

        auto count = tree.countIntervalsContainPoint(rightBoundaryPoint, true);
        REQUIRE(count == Test::boundaryIntervals().size());

        count = tree.countIntervalsContainPoint(rightBoundaryPoint, false);
        REQUIRE(0 == count);
    }


    SECTION("Outer and boundary intervals central point")
    {
        const auto intervals = Test::compositeIntervals(Test::outerIntervals(), Test::boundaryIntervals());
        const auto tree = IntervalTree(intervals);
        const auto count = tree.countIntervalsContainPoint(centralPoint);

        REQUIRE(count == intervals.size());
    }


    SECTION("Outer and boundary intervals left boundary point")
    {
        const auto intervals = Test::compositeIntervals(Test::outerIntervals(), Test::boundaryIntervals());
        const auto tree = IntervalTree(intervals);
        const auto count = tree.countIntervalsContainPoint(leftBoundaryPoint);

        REQUIRE(count == intervals.size());
    }


    SECTION("Outer and boundary intervals right boundary point")
    {
        const auto intervals = Test::compositeIntervals(Test::outerIntervals(), Test::boundaryIntervals());
        const auto tree = IntervalTree(intervals);
        const auto count = tree.countIntervalsContainPoint(rightBoundaryPoint);

        REQUIRE(count == intervals.size());
    }


    SECTION("Left intervals")
    {
        const auto tree = IntervalTree(Test::leftIntervals());
        const auto count = tree.countIntervalsContainPoint(centralPoint);

        REQUIRE(0 == count);
    }


    SECTION("Left and inner intervals")
    {
        const auto intervals = Test::compositeIntervals(Test::leftIntervals(), Test::innerIntervals());
        const auto tree = IntervalTree(intervals);
        const auto count = tree.countIntervalsContainPoint(centralPoint);

        REQUIRE(count == Test::innerIntervals().size());
    }


    SECTION("Right intervals")
    {
        const auto tree = IntervalTree(Test::rightIntervals());
        const auto count = tree.countIntervalsContainPoint(centralPoint);

        REQUIRE(0 == count);
    }


    SECTION("Right and inner intervals")
    {
        const auto intervals = Test::compositeIntervals(Test::rightIntervals(), Test::innerIntervals());
        const auto tree = IntervalTree(intervals);
        const auto count = tree.countIntervalsContainPoint(centralPoint);

        REQUIRE(count == Test::innerIntervals().size());
    }


    SECTION("Left overlapping intervals and left boundary point")
    {
        const auto tree = IntervalTree(Test::leftOverlappingIntervals());
        const auto count = tree.countIntervalsContainPoint(leftBoundaryPoint);

        REQUIRE(count == Test::leftOverlappingIntervals().size());
    }


    SECTION("Right overlapping intervals and right boundary point")
    {
        const auto tree = IntervalTree(Test::rightOverlappingIntervals());
        const auto count = tree.countIntervalsContainPoint(rightBoundaryPoint);

        REQUIRE(count == Test::rightOverlappingIntervals().size());
    }
}
