#include "Vector2.h"

Vector2::Vector2() : x(0), y(0)
{
}

Vector2::Vector2(double x, double y)
{
    this->x = x;
    this->y = y;
}

Vector2::~Vector2()
{
}

Vector2::Vector2(const Vector2& v)
{
    this->x = v.x;
    this->y = v.y;
}

Vector2 Vector2::operator+(Vector2& other)
{
    return Vector2(this->x + other.x, this->y + other.y);
}

Vector2 Vector2::operator*(double other)
{
    return Vector2(this->x * other, this->y * other);
}

Vector2 Vector2::operator-(Vector2& other)
{
    return Vector2(this->x - other.x, this->y - other.y);
}

double Vector2::Dot(Vector2& other)
{
    return x * other.x + y * other.y;
}

double Vector2::Magnitude()
{
    return sqrt(pow(this->x, 2) + pow(this->y, 2));
}

Vector2 Vector2::Rotate(double angle)
{
    double radian = angle * PI / 180.0f;
    return Vector2(x * cos(radian) - y * sin(radian), x * sin(radian) + y * cos(radian));
}

Vector2 Vector2::Normal()
{
    if (x == 0 && y == 0) {
        return Vector2(0, 0);
    }

    double d = this->Magnitude();
    return Vector2(x / d, y / d);
}
