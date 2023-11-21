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
    SFML_ERROR_NOT_RUNNING
} SFML_ERROR;

typedef enum sfmlDrawObjectType {
    SFML_OBJ_CIRCLE,
    SFML_OBJ_LINE
} SFML_DRAW_OBJECT_TYPE;

#define SFML_OBJECT_IDENTIFIER                  std::string;

class SfmlCoreWindow {
private:
    typedef struct sfmlObjectSpecs {
        bool                                    isDrawn;

        SFML_DRAW_OBJECT_TYPE                   type;

        uint32_t                                posX;
        uint32_t                                posY;

        uint32_t                                length;
        uint32_t                                height;

        uint32_t                                radius;

        sf::Color                               color;

        sf::CircleShape                         *circle;
        sf::RectangleShape                      *rectangle;
    } SFML_OBJECT, *PSFML_OBJECT;


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

    // Delete an object from the draw queue
    SfmlError DeleteDrawnObject(const SFML_OBJECT *obj);

private:
    void SFML_THREAD windowThread(void);

private:
    static sf::Color convertHexToSfmlColor(unsigned long color);
};

