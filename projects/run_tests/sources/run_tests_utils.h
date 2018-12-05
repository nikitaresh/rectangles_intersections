#ifndef RUN_TESTS_UTILS
#define RUN_TESTS_UTILS

#include <string>
#include <vector>
#include <rects.h>


class runTestsUtils
{
public:
    static bool parsInputeFile(const std::string& filePath, 
                               std::vector<ri::Rect>& rectangles);

    static bool parseAnswerFile(const std::string& filePath,
                                std::vector<ri::DerivedRect>& rectanglesAnswer,
                                std::vector<ri::RectsIntersection>& intersectionsAnswer);

    static std::vector<ri::DerivedRect>
        createFromRectArray(const std::vector<ri::Rect>& rectangles);

    static bool 
        compareRectangles(const std::vector<ri::DerivedRect>& rectangles1,
                          const std::vector<ri::DerivedRect>& rectangles2);

    static bool
        compareIntersections(const std::vector<ri::RectsIntersection>& intersections1,
                             const std::vector<ri::RectsIntersection>& intersections2);

    /**
    * @brief delete special symbols at the end of string,
             Linux strings of files contain '/r' at the end
    */
    static void deleteSpecialSymbolsAtTheEnd(std::string& str);

};

#endif // RUN_TESTS_UTILS