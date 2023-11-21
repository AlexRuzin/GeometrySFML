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
    SFML_DRAW_OBJECT_TYPE                   type;

    // circle, line
    uint32_t                                posX;
    uint32_t                                posY;

    // line
    uint32_t                                posX2;
    uint32_t                                posY2;

    uint32_t                                radius;

    sf::Color                               color;
    sf::Color                               color2;
} SFML_OBJECT, * PSFML_OBJECT;


class SfmlCoreWindow {
private:
    const uint32_t                              winWidth;
    const uint32_t                              winHeight;
    const std::string                           winName;

    EventSignal                                 windowThreadSync;
    std::thread                                 *windowThreadObj;

    sf::RenderWindow                            *renderWindow;

    std::mutex                                  drawObjectSync;
    std::list<SFML_OBJECT>                      drawObjectInput;

public:
    SfmlCoreWindow(uint32_t winWidth, uint32_t winHeight, std::string winName) :
        winWidth(winWidth),
        winHeight(winHeight),
        winName(winName),

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
    SfmlError DrawCircle(uint32_t x, uint32_t y, uint32_t radius, unsigned long color, SFML_OBJECT **objOut);

    SfmlError DrawLine(uint32_t x, uint32_t y, uint32_t x2, uint32_t y2, 
        unsigned long color, unsigned long color2, SFML_OBJECT ** objOut);

    // Delete an object from the draw queue
    SfmlError DeleteDrawnObject(const SFML_OBJECT *obj);

private:
    void SFML_THREAD windowThread(void);

private:
    static sf::Color convertHexToSfmlColor(unsigned long color);
};

