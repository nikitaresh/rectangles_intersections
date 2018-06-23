
#include <rects.h>
#include <algorithm>

using namespace ri;


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

bool ri::Rect::operator == (const Rect& other) const
{
    return (x == other.x && y == other.y && width == other.width && height == other.height);
}

bool ri::Rect::operator != (const Rect& other) const
{
    return !(*this == other);
}

bool ri::Rect::isValid() const
{
    return (width > 0 && height > 0);
}


ri::DerivedRect::DerivedRect()
{
}

ri::DerivedRect::DerivedRect(const DerivedRect& derivedRect)
    : index(derivedRect.index), rect(derivedRect.rect)
{
}

ri::DerivedRect::DerivedRect(DerivedRect&& derivedRect)
    : index(std::move(derivedRect.index)), rect(std::move(derivedRect.rect))
{
}

ri::DerivedRect::DerivedRect(const Rect& rect_, size_t index_)
    : index(index_), rect(rect_)
{
}

bool ri::DerivedRect::operator < ( const DerivedRect& other ) const
{
    return other.rect.y < rect.y;
}

bool ri::DerivedRect::operator == (const DerivedRect& other) const
{
    return (index == other.index && rect == other.rect);
}

bool ri::DerivedRect::operator != (const DerivedRect& other) const
{
    return !(*this == other);
}

DerivedRect& ri::DerivedRect::operator = ( const DerivedRect& other )
{
    index = other.index;
    rect = other.rect;
    return *this;
}

DerivedRect& ri::DerivedRect::operator = ( DerivedRect&& other ) noexcept
{
    index = std::move(other.index);
    rect = std::move(other.rect);
    return *this;
}


ri::RectsIntersection::RectsIntersection()
    : rectsIndexes(), rect()
{
}

ri::RectsIntersection::RectsIntersection( const RectsIntersection& intersection )
    : rectsIndexes(intersection.rectsIndexes), rect(intersection.rect)
{
}

ri::RectsIntersection::RectsIntersection( RectsIntersection&& intersection )
    : rectsIndexes(std::move(intersection.rectsIndexes)), rect(std::move(intersection.rect))
{
}

ri::RectsIntersection::RectsIntersection(const DerivedRect& derivedRect)
    : rect(derivedRect.rect)
{
    rectsIndexes.insert( derivedRect.index );
}


RectsIntersection& ri::RectsIntersection::operator = ( const RectsIntersection& other )
{
    rectsIndexes = other.rectsIndexes;
    rect = other.rect;
    return *this;
}

RectsIntersection& ri::RectsIntersection::operator = ( RectsIntersection&& other ) noexcept
{
    rectsIndexes = std::move( other.rectsIndexes );
    rect = std::move( other.rect );
    return *this;
}

bool ri::RectsIntersection::operator < ( const RectsIntersection& other ) const
{
    if (rectsIndexes.size() > other.rectsIndexes.size()) {
        return false;
    }

    if (rectsIndexes.size() < other.rectsIndexes.size()) {
        return true;
    }

    auto it1 = rectsIndexes.begin();
    auto it2 = other.rectsIndexes.begin();

    for( ; it1 != rectsIndexes.end() || it2 != other.rectsIndexes.end(); ++it1, ++it2 )
    {
        if (*it1 < *it2) {
            return true;
        }
        else if (*it1 > *it2) {
            return false;
        }
    }

    return true;
}

bool ri::RectsIntersection::operator == ( const RectsIntersection& other ) const
{
    if (rectsIndexes.size() != other.rectsIndexes.size()) {
        return false;
    }

    auto it1 = rectsIndexes.begin();
    auto it2 = other.rectsIndexes.begin();
    for (; it1 != rectsIndexes.end() || it2 != other.rectsIndexes.end(); ++it1, ++it2)
    {
        if (*it1 != *it2) {
            return false;
        }
    }

    return (rect == other.rect);
}

bool ri::RectsIntersection::operator != (const RectsIntersection& other) const
{
    return !(*this == other);
}

std::string ri::RectsIntersection::getIntersectionName() const
{
    std::string intersectionName;
    auto it = rectsIndexes.begin();
    while( it != rectsIndexes.end() )
    {
        intersectionName += std::to_string(*it);

        ++it;
        if( it == --rectsIndexes.end() ) {
            intersectionName += " and ";
        }
        else if (it != rectsIndexes.end()) {
            intersectionName += ", ";
        }
    }

    return intersectionName;
}

bool ri::RectsIntersection::addRectIndex(size_t index)
{
    auto it = rectsIndexes.find(index);
    if( it != rectsIndexes.end() ) {
        return false;
    }

    rectsIndexes.insert(index);
    return true;
}


Rect ri::intersectRect(const Rect& rect1, const Rect& rect2)
{
    int x1 = std::max(rect1.x, rect2.x);
    int y1 = std::max(rect1.y, rect2.y);
    int x2 = std::min(rect1.x + rect1.width, rect2.x + rect2.width);
    int y2 = std::min(rect1.y + rect1.height, rect2.y + rect2.height);

    return Rect(x1, y1, x2 - x1, y2 - y1);
}