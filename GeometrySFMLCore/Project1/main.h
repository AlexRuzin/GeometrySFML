#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "sfml.h"
#include "defines.h"

#define DEGREE_TO_RADIANS(x) (x * M_PI / 180)
#define RADIANS_TO_DEGREE(x) (x * (180.f / M_PI))

static const float centerX = SFML_WINDOW_SIZE_X / 2.f;
static const float centerY = SFML_WINDOW_SIZE_Y / 2.f;

class circleElement {
private:
    const float                         posX;
    const float                         posY;
    const float                         radius;


    const unsigned long                 backgroundColor;
    const unsigned long                 edgeColor;

    const float                         thickness;

    SFML_OBJECT                         *objectPtr;

    SfmlCoreWindow                      &coreWindow;

    const uint64_t                      index;
    float                               theta; // angle from the origin point (0, 0)

public:
    circleElement(
        SfmlCoreWindow &coreWindow, 
        uint64_t index,
        float x, float y, float radius, 
        unsigned long backgroundColor, unsigned long edgeColor, float thickness) :

        index(index),
        coreWindow(coreWindow),
        
        posX(x), posY(y),
        radius(radius),
        backgroundColor(backgroundColor), edgeColor(edgeColor),
        thickness(thickness),

        // We need the angle of this current circle from the origin point
        theta(getCircleTheta(x, y)),

        objectPtr(nullptr)
    {
        
    }

    ~circleElement(void)
    {
        if (objectPtr) {
            coreWindow.DeleteDrawnObject(objectPtr);
            objectPtr = nullptr;
        }
    }

    void DrawCircle(void)
    {
        SFML_OBJECT *ptr = nullptr;

        coreWindow.DrawCircle(
            posX, posY,
            radius,
            backgroundColor,
            edgeColor,
            thickness,
            &ptr);
        
        objectPtr = ptr;
    }    

    float GetPosX(void) const
    {
        return posX;
    }

    float GetPosY(void) const
    {
        return posY;
    }

    float GetTheta(void) const
    {
        return theta;
    }

private:
    static float getCircleTheta(float x, float y)
    {
        float theta = std::atan2f(y - centerY, x - centerX);

        theta *= 180 / M_PI;

        if (theta < 0) 
        {
            theta = 360.f + theta;
        }

        theta = std::roundf(theta);

        return theta;
    }
};