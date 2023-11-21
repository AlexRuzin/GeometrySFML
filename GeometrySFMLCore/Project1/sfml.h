#pragma once

#include "SFML/Graphics.hpp"
#include "defines.h"

#include <string>

enum SfmlError {
    SFML_ERROR_OK
};

class SfmlCoreWindow {
private:
    const uint32_t                              winWidth;
    const uint32_t                              winHeight;
    const std::string                           winName;

public:
    SfmlCoreWindow(uint32_t winWidth, uint32_t winHeight, std::string winName) :
        winWidth(winWidth),
        winHeight(winHeight),
        winName(winName)
    {

    }

    ~SfmlCoreWindow(void)
    {

    }

    SfmlError InitializeWindow(void);
};

