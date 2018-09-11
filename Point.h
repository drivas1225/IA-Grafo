#ifndef GRAPH_POINT_H
#define GRAPH_POINT_H

#include <cmath>
#include <iostream>
class Point{
    public:
         int x;
         int y;

        Point(){};

        int getX() const {
            return x;
        }

        void setX(int x) {
            this->x = x;
        }

        int getY() const {
            return y;
        }

        void setY(int y) {
            this->y = y;
        }

        float calculateDistance(Point other){
            float res = sqrt( pow( this->getX() - other.getX(), 2) + pow( this->getY() - other.getY(), 2) );
            return res;
        }

        inline bool operator==(const Point& other){
            return this->getX() == other.getX() and this->getY() == other.getY() ? true : false;
        }

    friend std::ostream& operator<<(std::ostream &out, const Point& p);
};

inline std::ostream& operator<<(std::ostream &out, const Point& p)
{
    out << " (" << p.x << " , " << p.y << ") ";
    return out;
}

#endif //GRAPH_POINT_H

