
#include <rectangles_intersections.h>
#include <intervaltree.hpp>
#include <iterator>
#include <algorithm>
#include <map>


using namespace ri;

// Rectangles ordered by right border Type
typedef std::map< int, std::vector<DerivedRect> > OrderedRectsType;

// Intervals::IntervalTree type
typedef Intervals::IntervalTree<int, OrderedRectsType> IntervalTreeType;

// interval type of Intervals::IntervalTree
typedef Intervals::Interval<int, OrderedRectsType> IntervalType;

// right borders storage type
typedef std::map< int, std::vector<IntervalType> > RightBordersType;



ri::Rect::Rect()
    : x(), y(), width(), height()
{
}

ri::Rect::Rect(const Rect& rect)
    : x(rect.x), y(rect.y), width(rect.width), height(rect.height)
{
}

ri::Rect::Rect(Rect&& rect)
    : x(rect.x), y(rect.y), width(rect.width), height(rect.height)
{
}

ri::Rect::Rect(int x_, int y_, int width_, int height_)
    : x(x_), y(y_), width(width_), height(height_)
{
}

Rect& ri::Rect::operator = (const Rect& other)
{
    x = other.x;
    y = other.y;
    width = other.width;
    height = other.height;
    return *this;
}

Rect& ri::Rect::operator = (Rect&& other) noexcept
{
    x = other.x;
    y = other.y;
    width = other.width;
    height = other.height;
    return *this;
}

bool ri::Rect::isValid() const
{
    return (width > 0 && height > 0);
}


ri::DerivedRect::DerivedRect()
{
}

ri::DerivedRect::DerivedRect(const DerivedRect& derivedRect)
    : name(derivedRect.name), rect(derivedRect.rect)
{
}

ri::DerivedRect::DerivedRect(DerivedRect&& derivedRect)
    : name(std::move(derivedRect.name)), rect(std::move(derivedRect.rect))
{
}

ri::DerivedRect::DerivedRect(const Rect& rect_, const std::string& name_)
    : name(name_), rect(rect_)
{
}

bool ri::DerivedRect::operator < ( const DerivedRect& other ) const
{
    return other.rect.y < rect.y;
}

DerivedRect& ri::DerivedRect::operator = ( const DerivedRect& other )
{
    name = other.name;
    rect = other.rect;
    return *this;
}

DerivedRect& ri::DerivedRect::operator = ( DerivedRect&& other ) noexcept
{
    name = std::move(other.name);
    rect = std::move(other.rect);
    return *this;
}


ri::RectsIntersection::RectsIntersection()
    : rectName1(), rectName2(), intersRect()
{
}

ri::RectsIntersection::RectsIntersection( const RectsIntersection& intersection )
    : rectName1(intersection.rectName1), rectName2(intersection.rectName2),
      intersRect(intersection.intersRect)
{
}

ri::RectsIntersection::RectsIntersection( RectsIntersection&& intersection )
    : rectName1(std::move(intersection.rectName1)), rectName2(std::move(intersection.rectName2)),
      intersRect(std::move(intersection.intersRect))
{
}

RectsIntersection& ri::RectsIntersection::operator = ( const RectsIntersection& other )
{
    rectName1 = other.rectName1;
    rectName2 = other.rectName2;
    intersRect = other.intersRect;
    return *this;
}

RectsIntersection& ri::RectsIntersection::operator = ( RectsIntersection&& other ) noexcept
{
    rectName1 = std::move( other.rectName1 );
    rectName2 = std::move( other.rectName2 );
    intersRect = std::move( other.intersRect );
    return *this;
}


Rect ri::intersectRect( const Rect& rect1, const Rect& rect2 )
{
    int x1 = std::max( rect1.x, rect2.x );
    int y1 = std::max( rect1.y, rect2.y );
    int x2 = std::min( rect1.x + rect1.width, rect2.x + rect2.width );
    int y2 = std::min( rect1.y + rect1.height, rect2.y + rect2.height );

    return Rect(x1, y1, x2 - x1, y2 - y1);
}



static void addRectangleToTree( IntervalTreeType& intervalTree,
                                RightBordersType& rightBorders,
                                const DerivedRect& drivedRect )
{
    int rightBorder = drivedRect.rect.x + drivedRect.rect.width;
    IntervalType searchInterval( drivedRect.rect.y, drivedRect.rect.y + drivedRect.rect.height );

    // update the rightBorders tree:
    auto mapIt = rightBorders.find( rightBorder );
    if( mapIt != rightBorders.end() ) {
        mapIt->second.push_back( searchInterval );
    }
    else
    {
        std::vector<IntervalType> intervalsVector { searchInterval };
        rightBorders.insert( std::make_pair(rightBorder, intervalsVector) );
    }

    // add a rectangle interval to intervalTree:
    auto interval = intervalTree.findInterval( searchInterval );
    if( interval != nullptr ) {
        interval->value[rightBorder].push_back( drivedRect );
    }
    else
    {
        std::vector<DerivedRect> derivedRects { drivedRect };
        searchInterval.value.insert( std::make_pair(rightBorder, derivedRects) );
        intervalTree.insert(searchInterval);
    }
}

static void processInterval( const IntervalType& interval,
                             IntervalTreeType& intervalTree,
                             const DerivedRect& derivedRect,
                             RightBordersType& rightBorders,
                             std::vector<RectsIntersection>& answer )
{
    for( auto it = interval.value.begin(); it != interval.value.end(); ++it )
    {
        const std::vector<DerivedRect>& derivedRects = it->second;
        for( size_t index = 0; index < derivedRects.size(); ++index )
        {
            // add new intersection to answer:
            RectsIntersection intersection;
            intersection.rectName1 = derivedRects[index].name;
            intersection.rectName2 = derivedRect.name;
            intersection.intersRect = ri::intersectRect( derivedRect.rect, derivedRects[index].rect );
            answer.push_back( intersection );

            // add new intersection to IntervalTree:
            DerivedRect derivedRect;
            derivedRect.name = intersection.rectName1 + ", " + intersection.rectName2;
            derivedRect.rect = intersection.intersRect;
            addRectangleToTree( intervalTree, rightBorders, derivedRect );
        }
    }
}

static void updateIntervalTree( int currentRectX,
                                IntervalTreeType& intervalTree,
                                RightBordersType& rightBorders )
{
    while( rightBorders.begin() != rightBorders.end() && rightBorders.begin()->first <= currentRectX )
    {
        for (auto vectorInterval : rightBorders.begin()->second)
        {
            IntervalType* interval = intervalTree.findInterval(vectorInterval);
            if (interval != nullptr)
            {
                interval->value.erase(rightBorders.begin()->first);
                if (interval->value.empty()) {
                    intervalTree.remove(vectorInterval);
                }
            }
        }

        rightBorders.erase(rightBorders.begin());
    }
}

std::vector<RectsIntersection> ri::calcRectsIntersections( const std::vector<Rect>& inputRects )
{
    std::vector<RectsIntersection> answer;  // rectangles intersections, answer
    IntervalTreeType intervalTree;          // interval tree that stores 'y' intervals of rectangles
    RightBordersType rightBorders;          // tree that store right border of rectangles to update
                                            // interval tree (delete a rect interval from intervalTree)

    std::vector<DerivedRect> namedRects(inputRects.size());
    for(size_t index = 0; index < namedRects.size(); ++index)
    {
        namedRects[index].name = std::to_string(index + 1);
        namedRects[index].rect = inputRects[index];
    }

    std::sort( namedRects.begin(), namedRects.end(),
               []( const DerivedRect& first, const DerivedRect& second ) {
        return first.rect.x < second.rect.x;
    } );

    for( size_t index = 0; index < namedRects.size(); ++index )
    {
        DerivedRect drivedRect;
        drivedRect.name = namedRects[index].name;
        drivedRect.rect = namedRects[index].rect;

        // remove all "finished" (drivedRect.left >= rect.right) rectangles from intervalTree:
        updateIntervalTree( drivedRect.rect.x, intervalTree, rightBorders );

        // calc new intersections for drivedRect:
        IntervalType rectInterval(drivedRect.rect.y, drivedRect.rect.y + drivedRect.rect.height);
        auto overlappingIntervals = intervalTree.findOverlappingIntervals( rectInterval );
        for( auto interval : overlappingIntervals ) {
            processInterval( interval, intervalTree, drivedRect, rightBorders, answer );
        }

        // add drivedRect to intervalTree:
        addRectangleToTree( intervalTree, rightBorders, drivedRect );
    }

    return answer;
}
