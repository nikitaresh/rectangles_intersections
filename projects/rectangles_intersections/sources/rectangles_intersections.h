#ifndef RECTANGLES_INTERSECTIONS
#define RECTANGLES_INTERSECTIONS

#include <string>
#include <vector>
#include <map>
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

private:
    void addRectangleToTree( IntervalTreeType& intervalTree,
                             RightBordersType& rightBorders,
                             const ri::RectsIntersection& intersection );

    void processNewRectangle( const IntervalType& interval,
                              IntervalTreeType& intervalTree,
                              const ri::DerivedRect& derivedRect,
                              RightBordersType& rightBorders,
                              std::vector<ri::RectsIntersection>& answer );

    void removeFinishedRectangles( int currentRectX,
                                   IntervalTreeType& intervalTree,
                                   RightBordersType& rightBorders );
};

#endif // RECTANGLES_INTERSECTIONS