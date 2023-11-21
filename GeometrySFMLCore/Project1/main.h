#pragma once

#include "sfml.h"
#include "defines.h"

class circleElement {
private:
    const float                         posX;
    const float                         posY;
    const float                         radius;


    const unsigned long                 backgroundColor;
    const unsigned long                 edgeColor;

    const float                         thickness;

    const float                         centerX;
    const float                         centerY;

    SFML_OBJECT                         *objectPtr;

    SfmlCoreWindow                      &coreWindow;

    const size_t                        index;
    float                               theta; // angle from the origin point (0, 0)

public:
    circleElement(SfmlCoreWindow &coreWindow, 
        int16_t index,
        float x, float y, float radius, 
        unsigned long backgroundColor, unsigned long edgeColor, float thickness) :

        index(index),
        coreWindow(coreWindow),
        
        posX(x), posY(y),
        radius(radius),
        backgroundColor(backgroundColor), edgeColor(edgeColor),
        thickness(thickness),

        // We need the angle of this current circle from the origin point
        centerX((SFML_WINDOW_SIZE_X / 2.f) - SFML_FLOWER_RADIUS),
        centerY((SFML_WINDOW_SIZE_Y / 2.f) - SFML_FLOWER_RADIUS),
        theta(std::atan2f(posX - centerX, posY - centerY)),

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
};