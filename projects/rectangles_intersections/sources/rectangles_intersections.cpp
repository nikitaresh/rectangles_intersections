
#include <rectangles_intersections.h>

using namespace ri;


RectanglesIntersections::RectanglesIntersections()
{
}

RectanglesIntersections::~RectanglesIntersections()
{
}

void RectanglesIntersections::addRectangleToTree( const RectsIntersection& intersection )
{
    int rightBorder = intersection.rect.x + intersection.rect.width;
    IntervalType searchInterval( intersection.rect.y, intersection.rect.y + intersection.rect.height );

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

    // Add a rectangle interval to intervalTree:
    auto interval = intervalTree.findInterval( searchInterval );
    if( interval != nullptr ) {
        interval->value[rightBorder].push_back( intersection );
    }
    else
    {
        std::vector<RectsIntersection> intersectionsVector { intersection };
        searchInterval.value.insert( std::make_pair(rightBorder, intersectionsVector) );
        intervalTree.insert(searchInterval);
    }
}

void RectanglesIntersections::processNewRectangle( const IntervalType& interval,
                                                   const DerivedRect& derivedRect,
                                                   std::vector<RectsIntersection>& answer )
{
    for( auto it = interval.value.begin(); it != interval.value.end(); ++it )
    {
        const std::vector<RectsIntersection>& intersectionsArr = it->second;
        for( size_t index = 0; index < intersectionsArr.size(); ++index )
        {
            // add new intersection to answer:
            RectsIntersection intersection;
            intersection.rectsIndexes = intersectionsArr[index].rectsIndexes;
            intersection.addRectIndex( derivedRect.index );
            intersection.rect = ri::intersectRect( derivedRect.rect, intersectionsArr[index].rect );
            answer.push_back( intersection );

            // add new intersection to IntervalTree:
            addRectangleToTree( intersection);
        }
    }
}

void RectanglesIntersections::removeFinishedRectangles( int currentRectX )
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

std::vector<RectsIntersection> 
RectanglesIntersections::calculate( const std::vector<Rect>& inputRects )
{
    intervalTree.clear();
    rightBorders.clear();

    std::vector<RectsIntersection> answer;  // rectangles intersections, answer

    std::vector<DerivedRect> namedRects(inputRects.size());
    for(size_t index = 0; index < namedRects.size(); ++index)
    {
        namedRects[index].index = index + 1;
        namedRects[index].rect = inputRects[index];
    }

    std::sort( namedRects.begin(), namedRects.end(),
               []( const DerivedRect& first, const DerivedRect& second ) {
        return first.rect.x < second.rect.x;
    } );

    for( size_t index = 0; index < namedRects.size(); ++index )
    {
        const DerivedRect& drivedRect = namedRects[index];

        // remove all "finished" (drivedRect.left >= rect.right) rectangles from intervalTree:
        removeFinishedRectangles( drivedRect.rect.x );

        // calc new intersections for drivedRect:
        IntervalType rectInterval(drivedRect.rect.y, drivedRect.rect.y + drivedRect.rect.height);
        auto overlappingIntervals = intervalTree.findOverlappingIntervals( rectInterval );
        for( auto interval : overlappingIntervals ) {
            processNewRectangle( interval, drivedRect, answer );
        }

        // add drivedRect to intervalTree:
        addRectangleToTree( RectsIntersection(drivedRect) );
    }

    return answer;
}


void RectanglesIntersections::printToStream(
    std::ostream& textStream,
    const std::vector<ri::Rect>& inputRects,
    const std::vector<ri::RectsIntersection>& intersections )
{
    textStream << "Input:" << std::endl;
    for (size_t index = 0; index < inputRects.size(); ++index)
    {
        const ri::Rect& rect = inputRects[index];
        textStream << "\t" << index + 1 << ": Rectangle at (" << rect.x << "," << rect.y
                   << "), w=" << rect.width << ", h=" << rect.height << "." << std::endl;
    }

    textStream << std::endl << "Intersections" << std::endl;
    for (size_t index = 0; index < intersections.size(); ++index)
    {
        const ri::RectsIntersection& rect = intersections[index];
        textStream << "\tBetween rectangle " << rect.getIntersectionName()
                   << " at (" << rect.rect.x << "," << rect.rect.y << "), w="
                   << rect.rect.width << ", h=" << rect.rect.height << "." << std::endl;
    }
}