#ifndef ALITA_VECTOR_2D_H
#define ALITA_VECTOR_2D_H

#include <cmath>

class Vector2D{
public:
    Vector2D(): m_x(0), m_y(0){}
    Vector2D(const Vector2D &rhs): m_x(rhs.m_x), m_y(rhs.m_y){}
    Vector2D(double x, double y): m_x(x), m_y(y){}

    double getX() const {return m_x;}
    double getY() const {return m_y;}

    void setX(double x) {m_x = x;}
    void setY(double y) {m_y = y;}

    double length() const {return sqrt(m_x * m_x + m_y * m_y);}

    bool isZero() const {return m_x == 0 && m_y == 0;}

    Vector2D &operator=(const Vector2D &rhs) {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        return *this;
    }

    Vector2D operator+(const Vector2D &rhs) const{
        return Vector2D(m_x + rhs.m_x, m_y + rhs.m_y);
    }

    Vector2D &operator+=(const Vector2D &rhs){
        m_x += rhs.m_x;
        m_y += rhs.m_y;
        return *this;
    }

    Vector2D operator-(const Vector2D &rhs) const{
        return Vector2D(m_x - rhs.m_x, m_y - rhs.m_y);
    }
    Vector2D &operator-=(const Vector2D &rhs){
        m_x -= rhs.m_x;
        m_y -= rhs.m_y;
        return *this;
    }

    Vector2D operator*(const double &rhs) const{
        return Vector2D(m_x * rhs, m_y * rhs);
    }

    Vector2D &operator*=(const double &rhs){
        m_x *= rhs;
        m_y *= rhs;
        return *this;
    }

    Vector2D operator/(const double &rhs) const {
        return Vector2D(m_x / rhs, m_y / rhs);
    }

    Vector2D &operator/=(const double &scalar){
        m_x /= scalar;
        m_y /= scalar;
        return *this;
    }

    void normalize(){
        if(!isZero())
            (*this) /= length();
    }

private:
    double m_x = 0.0f;
    double m_y = 0.0f;

};

bool isInsideBox(const Vector2D &p, const Vector2D &upperLeft, const Vector2D &lowerRight);

#endif