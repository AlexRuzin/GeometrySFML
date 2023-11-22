#pragma once

#include "SFML/Graphics.hpp"
#include "defines.h"
#include "signal.h"

#include <string>
#include <thread>
#include <memory>
#include <list>
#include <mutex>

#define SFML_THREAD                             __cdecl

typedef enum SfmlError {
    SFML_ERROR_OK,
    SFML_ERROR_INPUT,
    SFML_ERROR_SFML_INIT,
    SFML_ERROR_THREAD_ALREADY_RUNNING,
    SFML_ERROR_NOT_RUNNING,
    SFML_ERROR_OBJECT_DOES_NOT_EXIST
} SFML_ERROR;

typedef enum sfmlDrawObjectType {
    SFML_OBJ_CIRCLE,
    SFML_OBJ_LINE
} SFML_DRAW_OBJECT_TYPE;

typedef struct sfmlObjectSpecs {
    SFML_DRAW_OBJECT_TYPE                       type;

    // circle, line
    float                                       posX;
    float                                       posY;

    // line
    float                                       posX2;
    float                                       posY2;

    float                                       radius;

    float                                       thickness;

    sf::Color                                   color1; //for circle: background
    sf::Color                                   color2; // for circle: edge color
} SFML_OBJECT, * PSFML_OBJECT;


class SfmlCoreWindow {
private:
    const float                                 winWidth;
    const float                                 winHeight;
    const std::string                           winName;
    sf::Color                                   winBackgroundColor;

    EventSignal                                 windowThreadSync;
    std::thread                                 *windowThreadObj;

    sf::RenderWindow                            *renderWindow;

    std::mutex                                  drawObjectSync;
    std::list<SFML_OBJECT>                      drawObjectInput;

public:
    SfmlCoreWindow(float winWidth, float winHeight, std::string winName, unsigned long backgroundColor) :
        winWidth(winWidth),
        winHeight(winHeight),
        winName(winName),
        winBackgroundColor(convertHexToSfmlColor(backgroundColor)),

        renderWindow(nullptr),
        windowThreadObj(nullptr)
    {

    }

    ~SfmlCoreWindow(void)
    {

    }

    // Initialize SFML form
    SfmlError StartWindowThread(void);
    SfmlError StopWindowThread(void);

    // Color is in hexadecimal format (and alpha)
    SfmlError DrawCircle(float x, float y, 
        float radius, 
        unsigned long backgroundColor, unsigned long edgeColor, 
        float thickness,
        SFML_OBJECT **objOut);

    SfmlError DrawLine(float x, float y, float x2, float y2,
        unsigned long color, unsigned long color2, SFML_OBJECT **objOut);

    // Delete an object from the draw queue
    SfmlError DeleteDrawnObject(const SFML_OBJECT *obj);

    SfmlError DeleteAllDrawnObject(void);

private:
    void SFML_THREAD windowThread(void);

private:
    static sf::Color convertHexToSfmlColor(unsigned long color);
};

