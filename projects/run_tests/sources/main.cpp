#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <algorithm>
#include <rects.h>

#if WIN32
const std::string applicationName = "rectangles_intersections.exe";
#else
const std::string applicationName = "./rectangles_intersections";
#endif

using namespace ri;

/**
* @throw std::runtime_error or std::invalid_argument
*/
static Rect parseRectangle( const std::string& line )
{
    Rect rect;

    if( line[0] != '(' ) {
        throw std::runtime_error( "Bad string format" );
    }

    const size_t xEndIndex = line.find(',');
    if( xEndIndex == std::string::npos ) {
        throw std::runtime_error("Bad string format");
    }
    rect.x = std::stoi( line.substr(1, xEndIndex - 1) );

    const size_t closeBracketIndex = line.find( ')', xEndIndex );
    if( closeBracketIndex == std::string::npos ) {
        throw std::runtime_error("Bad string format");
    }

    rect.y = std::stoi(line.substr(xEndIndex + 1, closeBracketIndex - xEndIndex - 1));

    const size_t widthBeginingIndex = line.find("w=", closeBracketIndex);
    if( widthBeginingIndex == std::string::npos )
    {
        throw std::runtime_error("Bad string format");
    }

    const size_t widthEndIndex = line.find(',', widthBeginingIndex);
    if( widthEndIndex == std::string::npos ) {
        throw std::runtime_error("Bad string format");
    }

    rect.width = std::stoi(line.substr(widthBeginingIndex + 2, widthEndIndex - widthBeginingIndex - 2));

    const size_t heighBeginingIndex = line.find("h=", widthEndIndex);
    if( widthBeginingIndex == std::string::npos )
    {
        throw std::runtime_error("Bad string format");
    }

    const size_t heighEndIndex = line.find('.', heighBeginingIndex);
    if( heighEndIndex == std::string::npos ) {
        throw std::runtime_error("Bad string format");
    }

    rect.height = std::stoi(line.substr(heighBeginingIndex + 2, heighEndIndex - heighBeginingIndex - 2));
    return rect;
}

/**
* @throw std::runtime_error or std::invalid_argument
*/
static DerivedRect parseDerivedRect(const std::string& line)
{
    DerivedRect derivedRect;

    if( line[0] != '\t' ) {
        throw std::runtime_error("Bad string format");
    }

    const size_t colonIndex = line.find(':', 1);
    if( colonIndex == std::string::npos ) {
        throw std::runtime_error("Bad string format");
    }

    derivedRect.index = std::stoi(line.substr(1, colonIndex - 1));

    const size_t openBracketIndex = line.find('(', colonIndex);
    if( openBracketIndex == std::string::npos ) {
        throw std::runtime_error("Bad string format");
    }

    derivedRect.rect = parseRectangle(line.substr(openBracketIndex));
    return derivedRect;
}

static bool readRects(std::fstream& file, std::vector<DerivedRect>& rectangles)
{
    std::string line;
    std::getline(file, line);
    if( line != "Input:" ) {
        std::cout << "Error parse rectangles: bad file format" << std::endl;
        return false;
    }

    std::getline(file, line);
    while( file.good() && !line.empty() )
    {
        try {
            rectangles.push_back( parseDerivedRect(line) );
        }
        catch (...) {
            std::cout << "Error: bad line format of a rectangle line" << std::endl;
        }

        std::getline(file, line);
    }

    return true;
}


/**
* @throw std::runtime_error or std::invalid_argument
*/
static RectsIntersection parseIntersection(const std::string& line)
{
    RectsIntersection intersection;

    if( line.substr(0, 18) != "\tBetween rectangle" ) {
        throw std::runtime_error("Bad string format");
    }

    const size_t andIndex = line.find("and", 18);
    if( andIndex == std::string::npos ) {
        throw std::runtime_error("Bad string format");
    }

    size_t prevIndex = 18;
    size_t index = line.find(',', prevIndex);
    while (index < andIndex)
    {
        intersection.addRectIndex(std::stoi(line.substr(prevIndex, index - prevIndex)) );
        prevIndex = index + 1;
        index = line.find(',', prevIndex);
    }

    intersection.addRectIndex( std::stoi(line.substr(prevIndex, andIndex - prevIndex)) );

    const size_t atIndex = line.find("at", andIndex);
    if( atIndex == std::string::npos ) {
        throw std::runtime_error("Bad string format");
    }

    intersection.addRectIndex(std::stoi(line.substr(andIndex + 3, atIndex - andIndex - 3)));

    const size_t openBracketIndex = line.find('(', andIndex);
    if( openBracketIndex == std::string::npos ) {
        throw std::runtime_error("Bad string format");
    }

    intersection.rect = parseRectangle(line.substr(openBracketIndex));
    return intersection;
}

static bool readIntersections(std::fstream& file, std::vector<RectsIntersection>& intersections)
{
    std::string line;
    std::getline(file, line);
    if( line != "Intersections" ) {
        std::cout << "Error parse intersections: bad file format" << std::endl;
        return false;
    }

    while(file.good())
    {
        std::getline(file, line);
        if( line.empty() ) {
            continue;
        }
        try {
            intersections.push_back(parseIntersection(line));
        }
        catch (...) {
            std::cout << "Error: bad line format of a intersection line" << std::endl;
        }
    }

    return true;
}

static bool compareRectangles(std::vector<DerivedRect>& rectanglesAnswer,
                              std::vector<DerivedRect>& rectanglesBenchmark)
{
    if( rectanglesAnswer.size() != rectanglesBenchmark.size() )
    {
        std::cout << "different size of rectangles arrays" << std::endl;
        return false;
    }

    auto rectanglesSortFunction = [](const DerivedRect& first, const DerivedRect& second)
    {
        return first.index < second.index;
    };

    std::sort(rectanglesAnswer.begin(), rectanglesAnswer.end(), rectanglesSortFunction);
    std::sort(rectanglesBenchmark.begin(), rectanglesBenchmark.end(), rectanglesSortFunction);

    for (size_t index = 0; index < rectanglesAnswer.size(); ++index)
    {
        if (rectanglesAnswer[index] != rectanglesBenchmark[index]) {
            return false;
        }
    }

    return true;
}

static bool compareIntersections(std::vector<RectsIntersection>& intersectionsAnswer,
                                 std::vector<RectsIntersection>& intersectionsBenchmark)
{
    if( intersectionsAnswer.size() != intersectionsBenchmark.size() )
    {
        std::cout << "different size of intersections arrays" << std::endl;
        return false;
    }

    auto intersectionsSortFunction =
        [](const RectsIntersection& first, const RectsIntersection& second)
    {
        return first < second;
    };

    std::sort(intersectionsAnswer.begin(), intersectionsAnswer.end(), intersectionsSortFunction);
    std::sort(intersectionsBenchmark.begin(), intersectionsBenchmark.end(), intersectionsSortFunction);

    for (size_t index = 0; index < intersectionsAnswer.size(); ++index)
    {
        if (intersectionsAnswer[index] != intersectionsBenchmark[index]) {
            return false;
        }
    }

    return true;
}

static bool compareFiles(std::fstream& answerFile, std::fstream& benchmarkFile)
{
    if( !answerFile.is_open() ) {
        std::cout << "Can't open a temporary file" << std::endl;
        return false;
    }

    if( !benchmarkFile.is_open() ) {
        std::cout << "Can't open a benchmark file" << std::endl;
        return false;
    }


    std::vector<DerivedRect> rectanglesAnswer, rectanglesBenchmark;
    std::vector<RectsIntersection> intersectionsAnswer, intersectionsBenchmark;

    if( !readRects(answerFile, rectanglesAnswer) || !readIntersections(answerFile, intersectionsAnswer) ) {
        std::cout << "Error while parsing an answer file" << std::endl;
        return false;
    }

    if( !readRects(benchmarkFile, rectanglesBenchmark ) || 
        !readIntersections(benchmarkFile, intersectionsBenchmark))
    {
        std::cout << "Error while parsing a benchmark file" << std::endl;
        return false;
    }

    return compareRectangles(rectanglesAnswer, rectanglesBenchmark) &&
           compareIntersections(intersectionsAnswer, intersectionsBenchmark);
}

static bool runTest(const std::string& inputFilePath, const std::string& answerFilePath)
{
    const std::string temporaryOutputFile = "temporary_answer.txt";
    std::string command = applicationName + " " + inputFilePath + ">" + temporaryOutputFile;
    if( std::system(command.c_str()) != 0 ) {
        return false;
    }

    std::fstream file1(temporaryOutputFile, std::fstream::in);
    std::fstream file2(answerFilePath, std::fstream::in);

    bool cmpResult = compareFiles(file1, file2);

    file1.close();
    file2.close();

    return cmpResult;
}

static void runTests(std::fstream& testsFile)
{
    if( !testsFile.is_open() ) {
        std::cout << "A test file not found" << std::endl;
        return;
    }

    while( testsFile.good() )
    {
        std::string line;
        std::getline(testsFile, line);
        size_t spaceIndex = line.find(' ');
        if( spaceIndex == std::string::npos ) {
            std::cout << "Bad format of tests file" << std::endl;
            return;
        }

        std::string inputFilePath = line.substr(0, spaceIndex);
        std::string answerFilePath = line.substr(spaceIndex + 1);

        bool testResult = runTest(inputFilePath, answerFilePath);
        std::cout << "Test: " << inputFilePath << std::endl
                  << (testResult ? "OK" : "Failed") << std::endl;
    }
}

int main( int argc, char* argv[] )
{
    if( argc != 2 ) {
        std::cout << "Usage: run_tests <files_path>" << std::endl;
        return 1;
    }

    std::string filePath( argv[1] );

    std::fstream testsFile(filePath, std::fstream::in);
    runTests(testsFile);
    testsFile.close();

    return 0;
}