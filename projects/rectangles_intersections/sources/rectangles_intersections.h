#ifndef RECTANGLES_INTERSECTIONS
#define RECTANGLES_INTERSECTIONS

#include <string>
#include <vector>
#include <set>

namespace ri    // rectangles intersections namespace
{
    struct Rect
    {
        Rect();
        Rect( const Rect& rect );
        Rect( Rect&& rect );
        Rect( int x, int y, int width, int height );

        Rect& operator = ( const Rect& other );
        Rect& operator = ( Rect&& other ) noexcept;

        bool isValid() const;

    public:
        int x;
        int y;
        int width;
        int height;
    };

    struct DerivedRect
    {
        DerivedRect();
        DerivedRect( const DerivedRect& derivedRect );
        DerivedRect( DerivedRect&& derivedRect );
        DerivedRect( const Rect& rect, const std::string& name );

        bool operator < ( const DerivedRect& other ) const;
        DerivedRect& operator = ( const DerivedRect& other );
        DerivedRect& operator = ( DerivedRect&& other ) noexcept;

    public:
        std::string name;
        Rect rect;
    };

    struct RectsIntersection
    {
        RectsIntersection();
        RectsIntersection( const RectsIntersection& intersection );
        RectsIntersection( RectsIntersection&& intersection);

        RectsIntersection& operator = ( const RectsIntersection& other );
        RectsIntersection& operator = ( RectsIntersection&& other ) noexcept;

    public:
        std::string rectName1;
        std::string rectName2;
        Rect intersRect;
    };

    /**
    * @brief calculate intersection between two rectangles
    * @param rect1 - first rectangle
    * @param rect2 - second rectangle
    * @return intersection rectangle
    */
    Rect intersectRect( const Rect& rect1, const Rect& rect2 );

    /**
    * @brief calculate all intersections between input rectangles
    * @param rects - input rectangles
    * @return all rectangles intersections
    */
    std::vector<RectsIntersection> calcRectsIntersections( const std::vector<Rect>& rects );
}

#endif // RECTANGLES_INTERSECTIONS