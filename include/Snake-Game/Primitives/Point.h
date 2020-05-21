/* 
 * title: Primitives/Point.h
 * 
 */

#ifndef SNAKE_POINT
#define SNAKE_POINT

struct Point{
    int x, y;
    Point(int x=0, int y=0){
        this->x = x;
        this->y = y;
    }
    bool isValid(int width, int height) const{
        return x >= 0 && y >= 0 && x < width && y < height;
    }
    Point& operator+=(const Point& p){
        x += p.x;
        y += p.y;
        return *this;
    }
    Point operator+(const Point& p) const{
        Point ret = *this;
        ret += p;
        return ret;
    }
    bool operator<(const Point& p) const{
        if(x == p.x)
            return y < p.y;
        return x < p.x;
    }
    bool operator>(const Point& p) const{
        if(x == p.x)
            return y > p.y;
        return x > p.x;
    }
    bool operator==(const Point& p) const{
        return x == p.x && y == p.y;
    }
};

struct IndexedPoint{
    int idx;
    Point p;
    bool operator<(const IndexedPoint& ip) const{
        return idx < ip.idx;
    }
    bool operator>(const IndexedPoint& ip) const{
        return idx > ip.idx;
    }
    bool operator==(const IndexedPoint& ip) const{
        return idx == ip.idx;
    }
};

#endif