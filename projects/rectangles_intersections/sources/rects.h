#ifndef RECTS
#define RECTS

#include <string>
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

        bool operator == ( const Rect& other ) const;
        bool operator != ( const Rect& other ) const;

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
        DerivedRect( const Rect& rect, size_t index);

        bool operator < ( const DerivedRect& other ) const;
        bool operator == ( const DerivedRect& other ) const;
        bool operator != ( const DerivedRect& other ) const;

        DerivedRect& operator = ( const DerivedRect& other );
        DerivedRect& operator = ( DerivedRect&& other ) noexcept;

    public:
        size_t index;
        Rect rect;
    };

    struct RectsIntersection
    {
        RectsIntersection();
        RectsIntersection( const RectsIntersection& intersection );
        RectsIntersection( RectsIntersection&& intersection);
        RectsIntersection( const DerivedRect& derivedRect );

        RectsIntersection& operator = ( const RectsIntersection& other );
        RectsIntersection& operator = ( RectsIntersection&& other ) noexcept;

        bool operator < ( const RectsIntersection& other ) const;

        bool operator == ( const RectsIntersection& other ) const;
        bool operator != ( const RectsIntersection& other ) const;

        std::string getIntersectionName() const;

        bool addRectIndex(size_t index);

    public:
        std::set<size_t> rectsIndexes;
        Rect rect;
    };

    /**
    * @brief calculate intersection between two rectangles
    * @param rect1 - first rectangle
    * @param rect2 - second rectangle
    * @return intersection rectangle
    */
    Rect intersectRect(const Rect& rect1, const Rect& rect2);
}

#endif // RECTS
