
#include <run_tests_utils.h>
#include <iostream>
#include <fstream>
#include <exception>
#include <algorithm>
#include <json/reader.h>


static bool parseJsonRectangle(Json::Value& rectsValue, std::vector<ri::Rect>& rectangles)
{
    if (!rectsValue.isArray())
    {
        std::cout << "Error: bad format of json file: rects node isn't Array" << std::endl;
        return false;
    }

    for (Json::Value::iterator it = rectsValue.begin(); it != rectsValue.end(); ++it)
    {
        Json::Value node = *it;
        if (node.empty()) {
            std::cout << "Error: bad format of json file: empty line" << std::endl;
            return false;
        }

        bool isNodeCorrect = (node.isMember("x") && node["x"].isInt())
                          && (node.isMember("y") && node["y"].isInt())
                          && (node.isMember("w") && node["w"].isInt())
                          && (node.isMember("h") && node["h"].isInt());
        if (!isNodeCorrect)
        {
            std::cout << "Error: bad format of json file: bad line format" << std::endl;
            return false;
        }

        int x = node["x"].asInt(),
            y = node["y"].asInt(),
            w = node["w"].asInt(),
            h = node["h"].asInt();

        if (w <= 0 || h <= 0)
        {
            std::cout << "Error: bad format of json file: w <= 0 || h <= 0" << std::endl;
            return false;
        }

        rectangles.push_back(ri::Rect(x, y, w, h));
    }

    return true;
}

bool runTestsUtils::parsInputeFile(const std::string& filePath,
                                   std::vector<ri::Rect>& rectangles)
{
    std::fstream jsonFile(filePath, std::fstream::in);
    if (!jsonFile.is_open())
    {
        std::cout << "Error: can't open json file" << std::endl;
        return false;
    }

    Json::Value jsonRoot;
    Json::Reader jsonReader;
    bool parsingSuccessful = jsonReader.parse(jsonFile, jsonRoot);
    if (!parsingSuccessful)
    {
        std::cout << "Error: bad format of json file: can't parse file" << std::endl;
        return false;
    }

    jsonFile.close();

    Json::Value rectsValue = jsonRoot["rects"];
    if (rectsValue.empty())
    {
        std::cout << "Error: bad format of json file: can't find 'rects' node"
                  << std::endl;
        return false;
    }

    return parseJsonRectangle(rectsValue, rectangles);
};

//////////////////////////////////////////////////////////////////////////

/**
* @throw std::runtime_error or std::invalid_argument
*/
static ri::Rect parseRectangle(const std::string& line)
{
    ri::Rect rect;

    if (line.empty() || line[0] != '(') {
        throw std::runtime_error("Bad string format");
    }

    const size_t xEndIndex = line.find(',');
    if (xEndIndex == std::string::npos) {
        throw std::runtime_error("Bad string format");
    }
    rect.x = std::stoi(line.substr(1, xEndIndex - 1));

    const size_t closeBracketIndex = line.find(')', xEndIndex);
    if (closeBracketIndex == std::string::npos) {
        throw std::runtime_error("Bad string format");
    }

    rect.y = std::stoi(line.substr(xEndIndex + 1, closeBracketIndex - xEndIndex - 1));

    const size_t widthBeginingIndex = line.find("w=", closeBracketIndex);
    if (widthBeginingIndex == std::string::npos)
    {
        throw std::runtime_error("Bad string format");
    }

    const size_t widthEndIndex = line.find(',', widthBeginingIndex);
    if (widthEndIndex == std::string::npos) {
        throw std::runtime_error("Bad string format");
    }

    rect.width = std::stoi(line.substr(widthBeginingIndex + 2, widthEndIndex - widthBeginingIndex - 2));

    const size_t heighBeginingIndex = line.find("h=", widthEndIndex);
    if (widthBeginingIndex == std::string::npos)
    {
        throw std::runtime_error("Bad string format");
    }

    const size_t heighEndIndex = line.find('.', heighBeginingIndex);
    if (heighEndIndex == std::string::npos) {
        throw std::runtime_error("Bad string format");
    }

    rect.height = std::stoi( line.substr(heighBeginingIndex + 2, 
                                         heighEndIndex - heighBeginingIndex - 2) );
    return rect;
}

/**
* @throw std::runtime_error or std::invalid_argument
*/
static ri::DerivedRect parseDerivedRect(const std::string& line)
{
    ri::DerivedRect derivedRect;

    if (line.empty() || line[0] != '\t') {
        throw std::runtime_error("Bad string format");
    }

    const size_t colonIndex = line.find(':', 1);
    if (colonIndex == std::string::npos) {
        throw std::runtime_error("Bad string format");
    }

    derivedRect.index = std::stoi(line.substr(1, colonIndex - 1));

    const size_t openBracketIndex = line.find('(', colonIndex);
    if (openBracketIndex == std::string::npos) {
        throw std::runtime_error("Bad string format");
    }

    derivedRect.rect = parseRectangle(line.substr(openBracketIndex));
    return derivedRect;
}

static bool readRects(std::fstream& file, std::vector<ri::DerivedRect>& rectangles)
{
    if (!file.is_open())
    {
        std::cout << "Can't read rectangles: the file not opened" << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line);
    runTestsUtils::deleteSpecialSymbolsAtTheEnd(line);
    if (line != "Input:") {
        std::cout << "Error parse rectangles: bad file format" << std::endl;
        return false;
    }

    std::getline(file, line);
    runTestsUtils::deleteSpecialSymbolsAtTheEnd(line);
    while (file.good() && !line.empty())
    {
        try {
            rectangles.push_back(parseDerivedRect(line));
        }
        catch (...) {
            std::cout << "Error: bad line format of a rectangle line" << std::endl;
        }

        std::getline(file, line);
        runTestsUtils::deleteSpecialSymbolsAtTheEnd(line);
    }

    return true;
}


/**
* @throw std::runtime_error or std::invalid_argument
*/
static ri::RectsIntersection parseIntersection(const std::string& line)
{
    ri::RectsIntersection intersection;

    if (line.substr(0, 18) != "\tBetween rectangle") {
        throw std::runtime_error("Bad string format");
    }

    const size_t andIndex = line.find("and", 18);
    if (andIndex == std::string::npos) {
        throw std::runtime_error("Bad string format");
    }

    size_t prevIndex = 18;
    size_t index = line.find(',', prevIndex);
    while (index < andIndex)
    {
        intersection.addRectIndex(std::stoi(line.substr(prevIndex, index - prevIndex)));
        prevIndex = index + 1;
        index = line.find(',', prevIndex);
    }

    intersection.addRectIndex(std::stoi(line.substr(prevIndex, andIndex - prevIndex)));

    const size_t atIndex = line.find("at", andIndex);
    if (atIndex == std::string::npos) {
        throw std::runtime_error("Bad string format");
    }

    intersection.addRectIndex(
        std::stoi(line.substr(andIndex + 3, atIndex - andIndex - 3)) );

    const size_t openBracketIndex = line.find('(', andIndex);
    if (openBracketIndex == std::string::npos) {
        throw std::runtime_error("Bad string format");
    }

    intersection.rect = parseRectangle(line.substr(openBracketIndex));
    return intersection;
}

static bool readIntersections(std::fstream& file,
                              std::vector<ri::RectsIntersection>& intersections)
{
    if (!file.is_open())
    {
        std::cout << "Can't read intersections: the file not opened" << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line);
    runTestsUtils::deleteSpecialSymbolsAtTheEnd(line);
    if (line != "Intersections") {
        std::cout << "Error parse intersections: bad file format" << std::endl;
        return false;
    }

    while (file.good())
    {
        std::getline(file, line);
        runTestsUtils::deleteSpecialSymbolsAtTheEnd(line);
        if (line.empty()) {
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

bool 
  runTestsUtils::parseAnswerFile(const std::string& filePath,
                                 std::vector<ri::DerivedRect>& rectanglesAnswer,
                                 std::vector<ri::RectsIntersection>& intersectionsAnswer)
{
    std::fstream answerFile(filePath, std::fstream::in);
    if( !answerFile.is_open() )
    {
        std::cout << "Can't open a temporary file" << std::endl;
        return false;
    }

    if (!readRects(answerFile, rectanglesAnswer) ||
        !readIntersections(answerFile, intersectionsAnswer))
    {
        std::cout << "Error while parsing an answer file" << std::endl;
        return false;
    }

    answerFile.close();

    return true;
}

//////////////////////////////////////////////////////////////////////////

std::vector<ri::DerivedRect>
  runTestsUtils::createFromRectArray(const std::vector<ri::Rect>& rectangles)
{
    std::vector<ri::DerivedRect> drivedRects;
    drivedRects.reserve(rectangles.size());

    for( size_t index = 0; index < rectangles.size(); ++index ) {
        drivedRects.push_back(ri::DerivedRect(rectangles[index], index + 1));
    }

    return drivedRects;
}

//////////////////////////////////////////////////////////////////////////

bool runTestsUtils::compareRectangles(const std::vector<ri::DerivedRect>& rectangles1,
                                      const std::vector<ri::DerivedRect>& rectangles2)
{
    if( rectangles1.size() != rectangles2.size() ) {
        return false;
    }

    auto rectanglesSortFunction =
        [](const ri::DerivedRect& first, const ri::DerivedRect& second)
    {
        return first.index < second.index;
    };

    std::vector<ri::DerivedRect>
        rectanglesOne = rectangles1,
        rectanglesTwo = rectangles2;

    std::sort(rectanglesOne.begin(), rectanglesOne.end(),
              rectanglesSortFunction);
    std::sort(rectanglesTwo.begin(), rectanglesTwo.end(),
              rectanglesSortFunction);

    for (size_t index = 0; index < rectanglesOne.size(); ++index)
    {
        if (rectanglesOne[index] != rectanglesTwo[index]) {
            return false;
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

bool runTestsUtils::compareIntersections(
    const std::vector<ri::RectsIntersection>& intersections1,
    const std::vector<ri::RectsIntersection>& intersections2)
{
    if (intersections1.size() != intersections2.size()) {
        return false;
    }

    auto intersectionsSortFunction =
        [](const ri::RectsIntersection& first, const ri::RectsIntersection& second)
    {
        return first < second;
    };

    std::vector<ri::RectsIntersection>
        intersectionsOne = intersections1,
        intersectionsTwo = intersections2;

    std::sort(intersectionsOne.begin(), intersectionsOne.end(), intersectionsSortFunction);
    std::sort(intersectionsTwo.begin(), intersectionsTwo.end(), intersectionsSortFunction);

    for (size_t index = 0; index < intersectionsOne.size(); ++index)
    {
        if (intersectionsOne[index] != intersectionsTwo[index]) {
            return false;
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

static void deleteLastSymbol(std::string& str, char symbol)
{
    if (str.size() > 0 && str[str.size() - 1] == symbol) {
        str.resize((str.size() - 1));
    }
}

/**
* @brief delete special symbols at the end of string,
         Linux strings of files contain '/r' at the end
*/
void runTestsUtils::deleteSpecialSymbolsAtTheEnd(std::string& str)
{
    deleteLastSymbol(str, '\n');
    deleteLastSymbol(str, '\r');
}