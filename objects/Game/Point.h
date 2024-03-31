#ifndef POINT_H
#define POINT_H

#include "../Other/Object.h"

const int pointPosX = 10;
const int pointPosY = 10;

class Point : public Object
{
    void setText();
    int pointCount;

public:
    Point();
    ~Point();

    void init();
    void addPoint();
    int getCurrentPoint() const;
};

Point::Point()
{
    init();
}

void Point::init()
{
    mPosX = pointPosX;
    mPosY = pointPosY;

    pointCount = 0;
    setText();
}

void Point::setText()
{
    //Set text color as black
    static SDL_Color textColor = { 0, 0, 0, 255 };
    mTexture->loadFromRenderedText("Point: " + std::to_string(pointCount), firaFonts[FontStyle::Bold], textColor);
}

void Point::addPoint()
{
    pointCount++;
    setText();
}

int Point::getCurrentPoint() const
{
    return pointCount;
}

Point::~Point()
= default;


#endif //POINT_H
