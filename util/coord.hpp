#pragma once

#include <ostream>

namespace util{
    template<typename dimen_t>
    struct _coord2D{
        dimen_t y, x;

        _coord2D<dimen_t>& operator=(const _coord2D<dimen_t>& c){ this->y = c.y; this->x = c.x; return *this; }
        bool operator==(const _coord2D<dimen_t>& c) const{ return this->y == c.y && this->x == c.x; }
        bool operator<(const _coord2D<dimen_t>& c) const{ if( this->y != c.y ) return this->y < c.y; else return this->x < c.x; }

        friend std::ostream& operator<<(std::ostream& stream, const _coord2D<dimen_t>& coord) {
            return stream << '('<< coord.x << '*' << coord.y << ')';
        }

        _coord2D(dimen_t x, dimen_t y) : y(x), x(y){}
        _coord2D(const _coord2D<dimen_t>& c) : _coord2D(c.y, c.x){}

    };
}
