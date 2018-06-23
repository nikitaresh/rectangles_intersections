
#include <iostream>
#include <fstream>
#include <json/reader.h>
#include <rectangles_intersections.h>


static void printUsage()
{
    std::cout << "Usage: rectangles_intersections <json_path>" << std::endl;
}

static bool parseRectangle( Json::Value& rectsValue, std::vector<ri::Rect>& rectangles )
{
    if (!rectsValue.isArray())
    {
        std::cout << "Error: bad format of json file: rects node isn't Array" << std::endl;
        return false;
    }

    for( Json::Value::iterator it = rectsValue.begin(); it != rectsValue.end(); ++it )
    {
        Json::Value node = *it;
        if( node.empty() ) {
            std::cout << "Error: bad format of json file: empty line" << std::endl;
            return false;
        }

        bool isNodeCorrect =
            (node.isMember("x") && node["x"].isInt()) && (node.isMember("y") && node["y"].isInt())
            && (node.isMember("w") && node["w"].isInt()) && (node.isMember("h") && node["h"].isInt());
        if( !isNodeCorrect )
        {
            std::cout << "Error: bad format of json file: bad line format" << std::endl;
            return false;
        }

        int x = node["x"].asInt(),
            y = node["y"].asInt(),
            w = node["w"].asInt(),
            h = node["h"].asInt();

        if( w <= 0 || h <= 0 )
        {
            std::cout << "Error: bad format of json file: w <= 0 || h <= 0" << std::endl;
            return false;
        }

        rectangles.push_back( ri::Rect(x, y, w, h) );
    }

    return true;
}

static bool parseJson( const std::string& filePath, std::vector<ri::Rect>& rectangles )
{
    std::fstream jsonFile( filePath, std::fstream::in );
    if( !jsonFile.is_open() )
    {
        std::cout << "Error: can't open json file" << std::endl;
        return false;
    }

    Json::Value jsonRoot;
    Json::Reader jsonReader;
    bool parsingSuccessful = jsonReader.parse( jsonFile, jsonRoot );
    if( !parsingSuccessful )
    {
        std::cout << "Error: bad format of json file: can't parse file" << std::endl;
        return false;
    }

    Json::Value rectsValue = jsonRoot["rects"];
    if( rectsValue.empty() )
    {
        std::cout << "Error: bad format of json file: can't find 'rects' node" << std::endl;
        return false;
    }

    return parseRectangle( rectsValue, rectangles );
}

static void printAnswer( const std::vector<ri::Rect>& inputRects,
                         const std::vector<ri::RectsIntersection>& intersections )
{
    std::cout << "Input:" << std::endl;
    for (size_t index = 0; index < inputRects.size(); ++index)
    {
        const ri::Rect& rect = inputRects[index];
        std::cout << "\t" << index + 1 << ": Rectangle at (" << rect.x << "," << rect.y 
                  << "), w=" << rect.width << ", h=" << rect.height << "." << std::endl;
    }

    std::cout << std::endl << "Intersections" << std::endl;
    for (size_t index = 0; index < intersections.size(); ++index)
    {
        const ri::RectsIntersection& rect = intersections[index];
        std::cout << "\tBetween rectangle " << rect.getIntersectionName()
                  << " at (" << rect.rect.x << "," << rect.rect.y << "), w="
                  << rect.rect.width << ", h=" << rect.rect.height << "." << std::endl;
    }
}


int main( int argc, char* argv[] )
{
    if( argc != 2 )
    {
        printUsage();
        return 1;
    }

    std::string filePath(argv[1]);
    std::vector<ri::Rect> rectangles;
    if( !parseJson( filePath, rectangles ) )
    {
        std::cout << "Error while parse JSON" << std::endl;
        return 1;
    }

    // According to the problem description: "You may discard the remaining rectangles after the first 1000".
    const int maxNumOfRectanples = 1000;
    if( rectangles.size() > maxNumOfRectanples) {
        rectangles.resize( maxNumOfRectanples );
    }

    RectanglesIntersections rectanglesIntersections;
    std::vector<ri::RectsIntersection> intersections = rectanglesIntersections.calculate( rectangles );
    printAnswer(rectangles, intersections);

    return 0;
}

