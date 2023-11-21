#pragma once

#include "SFML/Graphics.hpp"
#include "defines.h"
#include "signal.h"

#include <string>
#include <thread>

#define SFML_THREAD                             __cdecl

enum SfmlError {
    SFML_ERROR_OK,
    SFML_ERROR_SFML_INIT,
    SFML_ERROR_THREAD_ALREADY_RUNNING,
    SFML_ERROR_NOT_RUNNING
};

class SfmlCoreWindow {
private:
    const uint32_t                              winWidth;
    const uint32_t                              winHeight;
    const std::string                           winName;

    EventSignal                                 windowThreadSync;
    std::thread                                 *windowThreadObj;

    sf::RenderWindow                            *renderWindow;

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

    SfmlError StartWindowThread(void);
    SfmlError StopWindowThread(void);

private:
    void SFML_THREAD windowThread(void);
};

