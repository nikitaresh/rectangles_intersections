#ifndef RECTANGLES_INTERSECTIONS
#define RECTANGLES_INTERSECTIONS

#include <string>
#include <vector>
#include <map>
#include <ostream>
#include <rects.h>
#include <intervaltree.hpp>


// Rectangles ordered by right border Type
typedef std::map< int, std::vector<ri::RectsIntersection> > OrderedRectsType;

// Intervals::IntervalTree type
typedef Intervals::IntervalTree<int, OrderedRectsType> IntervalTreeType;

// interval type of Intervals::IntervalTree
typedef Intervals::Interval<int, OrderedRectsType> IntervalType;

// right borders storage type
typedef std::map< int, std::vector<IntervalType> > RightBordersType;



class RectanglesIntersections
{
public:
    RectanglesIntersections();
    ~RectanglesIntersections();

    /**
    * @brief calculate all intersections between input rectangles
    * @param rects - input rectangles
    * @return all rectangles intersections
    */
    std::vector<ri::RectsIntersection> calculate(const std::vector<ri::Rect>& rects);

    static void printToStream(std::ostream& textStream,
                              const std::vector<ri::Rect>& inputRects,
                              const std::vector<ri::RectsIntersection>& intersections);

private:
    void addRectangleToTree( const ri::RectsIntersection& intersection );

    void processNewRectangle( const IntervalType& interval,
                              const ri::DerivedRect& derivedRect,
                              std::vector<ri::RectsIntersection>& answer );

    void removeFinishedRectangles( int currentRectX );

private:
    IntervalTreeType intervalTree;  // interval tree that stores 'y' intervals of rectangles
    RightBordersType rightBorders;  // tree that store right border of rectangles to update
                                    // interval tree (delete a rect interval from intervalTree)


};

#endif // RECTANGLES_INTERSECTIONS