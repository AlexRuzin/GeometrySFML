#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "sfml.h"
#include "defines.h"

#define DEGREE_TO_RADIANS(x) (x * M_PI / 180)
#define RADIANS_TO_DEGREE(x) (x * (180.f / M_PI))

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

static const float centerX = SFML_WINDOW_SIZE_X / 2.f;
static const float centerY = SFML_WINDOW_SIZE_Y / 2.f;

class circleElement {
private:
    const float                         posX;
    const float                         posY;
    const float                         radius;


    unsigned long                       backgroundColor;
    const unsigned long                 edgeColor;

    const float                         thickness;

    SFML_OBJECT                         *objectPtr;

    SfmlCoreWindow                      &coreWindow;

    const uint16_t                      round;
    float                               theta; // angle from the origin point (0, 0)

    // Visual radius of the circle, not to be confused with radius that is the actual radius
    //  This radius just changes the visual size of the circle
    float                               visualRadius;

public:
    circleElement(
        SfmlCoreWindow &coreWindow, 
        uint16_t round,
        float x, float y, float radius, 
        unsigned long backgroundColor, unsigned long edgeColor, float thickness) :

        round(round),
        coreWindow(coreWindow),
        
        posX(x), posY(y),
        radius(radius), visualRadius(radius),
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
        if (objectPtr) {
            coreWindow.DeleteDrawnObject(objectPtr);
            objectPtr = nullptr;
        }

        SFML_OBJECT *ptr = nullptr;

        coreWindow.DrawCircle(
            posX, posY,
            visualRadius,
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

    void IncVisualRadius(float n)
    {
        visualRadius += n;
    }

    void SetVisualRadius(float n)
    {
        visualRadius = n;
    }

    float GetVisualRadius(void) const
    {
        return visualRadius;
    }

    void SetBackgroundColor(unsigned long color)
    {
        backgroundColor = color;
    }

    unsigned long GetBackgroundColor(unsigned long color) const
    {
        return backgroundColor;
    }

    uint16_t GetRound(void) const
    {
        return round;
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

typedef struct {
    float h;
    float s;
    float l;
} HSL, * PHSL;