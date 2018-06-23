#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <rects.h>

using namespace ri;

static void generateInput(int numRectangles, std::vector<Rect>& rectangles)
{
    rectangles.resize(numRectangles);
    for( int index = 0; index < numRectangles; ++index )
    {
        int x = 10 + 5 * (index % 100);
        int y = 10 + 200 * (index / 100);
        int w = 50;
        int h = 100;
        rectangles[index] = Rect(x, y, w, h);
    }
}

/**
* @brief Naive implementation of the rectangles intersections algorithm
* @param rects - input rectangles
* @return all rectangles intersections
*/
static std::vector<RectsIntersection> calcRectsIntersections(const std::vector<Rect>& rects)
{
    std::vector<RectsIntersection> answer;          // rectangles intersections, answer
    std::vector<RectsIntersection> allRectangles;   // all intersactions + input rectangles

    for( size_t rectsIndex = 0; rectsIndex < rects.size(); ++rectsIndex )
    {
        std::vector<RectsIntersection> newIntersections;  // intersection in the current step

        DerivedRect derivedRect(rects[rectsIndex], rectsIndex + 1);

        for( size_t index = 0; index < allRectangles.size(); ++index )
        {
            Rect rect = intersectRect(derivedRect.rect, allRectangles[index].rect);
            if( rect.isValid() )
            {
                RectsIntersection intersection;
                intersection.rectsIndexes = allRectangles[index].rectsIndexes;
                intersection.addRectIndex( derivedRect.index );
                intersection.rect = rect;

                newIntersections.push_back( intersection );
                answer.push_back( intersection );
            }
        }

        allRectangles.push_back(derivedRect);
        std::copy( newIntersections.begin(), newIntersections.end(),
                   std::back_inserter(allRectangles) );
    }

    return answer;
}


static bool writeInputToFile(std::fstream& file, const std::vector<Rect>& rectangles)
{
    if( !file.is_open() ) {
        std::cout << "Error: unable to create output file" << std::endl;
        return false;
    }

    file << "{" << std::endl;
    file << "    \"rects\": [" << std::endl;

    for( size_t index = 0; index < rectangles.size(); ++index )
    {
        int x = rectangles[index].x;
        int y = rectangles[index].y;
        int w = rectangles[index].width;
        int h = rectangles[index].height;
        file << "        {\"x\": " << x << ", \"y\": " << y << ", \"w\": " << w 
             << ", \"h\": " << h << (index == rectangles.size() -1 ? " }" : " },") << std::endl;
    }

    file << "    ]" << std::endl;
    file << "}" << std::endl;

    return true;
}

static bool writeAnswerToFile(std::fstream& file, 
                              const std::vector<Rect>& rectangles,
                              const std::vector<RectsIntersection>& intersections)
{
    if( !file.is_open() ) {
        std::cout << "Error: unable to create answer output file" << std::endl;
        return false;
    }

    file << "Input:" << std::endl;
    for (size_t index = 0; index < rectangles.size(); ++index)
    {
        const ri::Rect& rect = rectangles[index];
        file << "\t" << index + 1 << ": Rectangle at (" << rect.x << "," << rect.y
             << "), w=" << rect.width << ", h=" << rect.height << "." << std::endl;
    }

    file << std::endl << "Intersections" << std::endl;
    for (size_t index = 0; index < intersections.size(); ++index)
    {
        const ri::RectsIntersection& rect = intersections[index];
        file << "\tBetween rectangle " << rect.getIntersectionName()
             << " at (" << rect.rect.x << "," << rect.rect.y << "), w="
             << rect.rect.width << ", h=" << rect.rect.height << "." << std::endl;
    }

    return true;
}

static bool writeToFiles(const std::string& outputFileSuffix,
                         const std::vector<Rect>& rectangles,
                         const std::vector<RectsIntersection>& intersections)
{
    std::string inputFileName = "test" + outputFileSuffix + ".json";
    std::fstream inputFile(inputFileName, std::fstream::out);
    bool writeInputResult = writeInputToFile(inputFile, rectangles);
    inputFile.close();

    std::string answerFileName = "answer" + outputFileSuffix + ".txt";
    std::fstream answerFile(answerFileName, std::fstream::out);
    bool writeAnswerResult = writeAnswerToFile(answerFile, rectangles, intersections);
    answerFile.close();

    return writeInputResult && writeAnswerResult;
}

int main( int argc, char* argv[] )
{
    if ( argc != 3 ) {
        std::cout << "Usage: generate_input <files_suffix> <num_rectangles>" << std::endl;
        return 1;
    }

    std::string outputFileSuffix( argv[1] );
    int numRectangles = 0;
    try {
        numRectangles = std::stoi( std::string(argv[2]) );
    }
    catch (...) {
        std::cout << "Error: wrong format of <num_rectangles>" << std::endl;
        return 1;
    }

    std::vector<Rect> rectangles;
    generateInput(numRectangles, rectangles);

    std::vector<RectsIntersection> intersections = calcRectsIntersections(rectangles);

    bool writeResult = writeToFiles(outputFileSuffix, rectangles, intersections);

    std::string message = writeResult ? "File were recorded successful"
                                      : "An error while writing files";
    std::cout << message << std::endl;

    return 0;
}
