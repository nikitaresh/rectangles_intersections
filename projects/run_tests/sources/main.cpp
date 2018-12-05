
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <gtest/gtest.h>
#include <rects.h>
#include <run_tests_utils.h>
#include <rectangles_intersections.h>


TEST(RectanglesIntersectionsTest, ArgumentsTests)
{
    RectanglesIntersections rectanglesIntersections;
    std::vector<ri::Rect> rectangles;

    EXPECT_TRUE(rectanglesIntersections.calculate(rectangles).empty());

    ri::Rect rect(0, 0, 100, 100);

    int numCycles = 10;
    for (int index = 0; index < numCycles; ++index)
    {
        rectangles.push_back(rect);

        const int numCompleteIntersections =
            static_cast<int>(std::pow(2, index + 1)) - (index + 1) - 1;
        const int numIntersections =
            static_cast<int>(rectanglesIntersections.calculate(rectangles).size());
        EXPECT_TRUE( numCompleteIntersections  == numIntersections );
    }
}

//////////////////////////////////////////////////////////////////////////

static bool getTestInfo(std::fstream& testsListFile,
                        std::string& testName,
                        std::string& benchmarkName)
{
    std::string line;
    std::getline(testsListFile, line);
    runTestsUtils::deleteSpecialSymbolsAtTheEnd(line);

    std::stringstream filesStream(line);
    filesStream >> testName >> benchmarkName;

    return !filesStream.fail();
}

static bool runTest(const std::string& inputFilepath, const std::string& benchmarkFilepath)
{
    std::vector<ri::Rect> rectangles;
    std::vector<ri::DerivedRect> rectanglesBenchmark;
    std::vector<ri::RectsIntersection> intersectionsBenchmark;

    if (!runTestsUtils::parsInputeFile(inputFilepath, rectangles))
    {
        std::cout << "Error while parsing an input file" << std::endl;
        return false;
    }

    if (!runTestsUtils::parseAnswerFile(benchmarkFilepath, rectanglesBenchmark,
                                        intersectionsBenchmark))
    {
        std::cout << "Error while parsing an answer file" << std::endl;
        return false;
    }

    std::vector<ri::DerivedRect> inpRects =
        runTestsUtils::createFromRectArray(rectangles);

    RectanglesIntersections rectanglesIntersections;
    std::vector<ri::RectsIntersection> intersections =
        rectanglesIntersections.calculate(rectangles);

    return runTestsUtils::compareRectangles(inpRects, rectanglesBenchmark) &&
           runTestsUtils::compareIntersections(intersections, intersectionsBenchmark);
}

TEST(RectanglesIntersectionsTest, UseCaseTests)
{
    const std::string testsListFilepath = "../data/tests_list.txt";
    std::fstream testsListFile(testsListFilepath, std::fstream::in);
    EXPECT_TRUE( testsListFile.is_open() );

    while( testsListFile.good() )
    {
        std::string testName, benchmarkName;
        EXPECT_TRUE(getTestInfo(testsListFile, testName, benchmarkName));
        std::cout << "Test: " << testName << std::endl;
        EXPECT_TRUE(runTest(testName, benchmarkName));
    }

    testsListFile.close();
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}