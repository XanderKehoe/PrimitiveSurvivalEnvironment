#pragma once
#include <complex>

class Vector2DFloat {
public:
    Vector2DFloat(float x, float y) : x_(x), y_(y) {}

    float x() const { return x_; }
    float y() const { return y_; }

    void setX(float x) { x_ = x; }
    void setY(float y) { y_ = y; }

    float length() const 
    {
        return std::sqrt(x_ * x_ + y_ * y_);
    }

    void normalize() 
    {
        float l = length();
        if (l > 0) {
            x_ /= l;
            y_ /= l;
        }
    }

    void add(const Vector2DFloat& other) 
    {
        x_ += other.x_;
        y_ += other.y_;
    }

    void rotate(float degrees) 
    {
        float radians = (float) (degrees * M_PI / 180.0);
        float c = std::cos(radians);
        float s = std::sin(radians);
        float x = x_;
        float y = y_;
        x_ = x * c - y * s;
        y_ = x * s + y * c;
    }

private:
    float x_, y_;
};