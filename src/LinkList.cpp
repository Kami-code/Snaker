#include <iostream>
#include "header/LinkList.h"

Point::Point(){
    x = 0; y = 0;
}

bool Point::operator > (const Point &a) const {
    if (x != a.x) return x > a.x;
    else return y > a.y;
}

bool Point::operator < (const Point &a) const {
    if (x != a.x) return x < a.x;
    else return y < a.y;
}

bool Point::operator == (const Point &a) const {
    return (x == a.x) && (y == a.y);
}

Point Point::operator + (const Point &a) const {
    Point c(x + a.x, y + a.y);
    return c;
}
Point Point::operator - (const Point &a) const {
    Point c(x - a.x, y - a.y);
    return c;
}

ostream& Point::operator << (ostream & os) {
    os << "( " << x << " , " << y << " ) ";
    return os;
}
