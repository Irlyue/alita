#include "Vector2D.h"


bool isInsideBox(const Vector2D &p, const Vector2D &upperLeft, const Vector2D &lowerRight){
    return (p.getX() >= upperLeft.getX() && p.getY() >= upperLeft.getY()) && 
        (p.getX() < lowerRight.getX() && p.getY() < lowerRight.getY());
}