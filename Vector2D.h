#ifndef ALITA_VECTOR_2D_H
#define ALITA_VECTOR_2D_H

#include <cmath>

class Vector2D{
public:
    Vector2D(): m_x(0), m_y(0){}
    Vector2D(float x, float y): m_x(x), m_y(y){}

    float getX() const {return m_x;}
    float getY() const {return m_y;}

    void setX(float x) {m_x = x;}
    void setY(float y) {m_y = y;}

    float length() const {return sqrt(m_x * m_x + m_y * m_y);}

    bool isZero() const {return m_x == 0 and m_y == 0;}

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

    Vector2D operator*(const float &rhs) const{
        return Vector2D(m_x * rhs, m_y * rhs);
    }

    Vector2D &operator*=(const float &rhs){
        m_x *= rhs;
        m_y *= rhs;
        return *this;
    }

    Vector2D operator/(const float &rhs) const {
        return Vector2D(m_x / rhs, m_y / rhs);
    }

    Vector2D &operator/=(const float &scalar){
        m_x /= scalar;
        m_y /= scalar;
        return *this;
    }

    void normalize(){
        if(!isZero())
            (*this) /= length();
    }

private:
    float m_x = 0.0f;
    float m_y = 0.0f;

};

#endif