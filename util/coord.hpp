#pragma once

namespace util{
    template<typename dimen_t>
    struct _coord{
        dimen_t y, x;

        _coord<dimen_t>& operator=(const _coord<dimen_t>& c){ this->y = c.y; this->x = c.x; return *this; }
        bool operator==(const _coord<dimen_t>& c) const{ return this->y == c.y && this->x == c.x; }
        bool operator<(const _coord<dimen_t>& c) const{ if( this->y != c.y ) return this->y < c.y; else return this->x < c.x; }
            
        _coord(dimen_t x, dimen_t y) : y(x), x(y){}
        _coord(const _coord<dimen_t>& c) : _coord(c.y, c.x){}

    };
}
