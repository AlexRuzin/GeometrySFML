#include "sfml.h"

#include "SFML/Graphics.hpp"

#include <string>
#include <thread>
#include <mutex>

SfmlError SfmlCoreWindow::StartWindowThread(void)
{
    if (windowThreadObj) {
        return SFML_ERROR_THREAD_ALREADY_RUNNING;
    }

    windowThreadSync.ResetSignal();

    windowThreadObj = new std::thread(&SfmlCoreWindow::windowThread, this);
    windowThreadObj->detach();

    return SFML_ERROR_OK;
}

SfmlError SfmlCoreWindow::StopWindowThread(void)
{
    if (!windowThreadObj) {
        return SFML_ERROR_NOT_RUNNING;
    }

    windowThreadSync.SetSignal();
    if (windowThreadObj->joinable()) {
        windowThreadObj->join();
    }

    delete windowThreadObj;

    return SFML_ERROR_OK;
}

void SFML_THREAD SfmlCoreWindow::windowThread(void)
{
    if (renderWindow) {
        return;
    }

    renderWindow = new sf::RenderWindow(sf::VideoMode(winWidth, winHeight), winName);

#if 0
    sf::CircleShape circle(100);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(350, 250); // position the circle in the center

    // Define the radius line as a thin rectangle
    sf::RectangleShape radiusLine;
    float radius = circle.getRadius();
    radiusLine.setSize(sf::Vector2f(radius, 2)); // Width of 2 pixels and length of the radius
    radiusLine.setFillColor(sf::Color::Red); // Red color for the radius line

    // Calculate the position and rotation for the radius line
    sf::Vector2f circleCenter = circle.getPosition();
    circleCenter.x += radius; // Adjust for the top-left origin of the circle
    circleCenter.y += radius;
    radiusLine.setPosition(circleCenter);
    radiusLine.setOrigin(0, 1); // Set origin to one of the ends of the line
#endif

    while (renderWindow->isOpen() && !windowThreadSync.IsSignaled()) {
        std::unique_lock<std::mutex> mlock(drawObjectSync);

        // Event processing
        sf::Event event;
        while (renderWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                renderWindow->close();
        }

        // Rendering
        renderWindow->clear(sf::Color::Black);

        for (std::list<SFML_OBJECT>::iterator i = drawObjectInput.begin(); i != drawObjectInput.end(); i++) {
            if (!i->isDrawn) {
                
                i->isDrawn = true;

                switch(i->type) {
                case SFML_OBJ_CIRCLE:
                    i->circle = new sf::CircleShape(i->radius);
                    i->circle->setFillColor(i->color);
                    i->circle->setPosition(i->posX, i->posY);

                    break;
                case SFML_OBJ_LINE:


                    break;
                default:
                    break;
                }               
            }
        }

#if 0
        renderWindow->draw(circle);
        renderWindow->draw(radiusLine);
#endif

        renderWindow->display();
    }

    renderWindow->close();
    delete renderWindow;
    renderWindow = nullptr;

    return;
}

SfmlError SfmlCoreWindow::DrawCircle(uint32_t x, uint32_t y, uint32_t radius, unsigned long color, SFML_OBJECT **objOut)
{
    std::unique_lock<std::mutex> mlock(drawObjectSync);

    if (!renderWindow) {
        return SFML_ERROR_NOT_RUNNING;
    }

    SFML_OBJECT obj;
    obj.isDrawn = false;
    obj.type = SFML_OBJ_CIRCLE;
    obj.posX = x;
    obj.posY = y;
    obj.radius = radius;
    obj.color = convertHexToSfmlColor(color);

    drawObjectInput.push_back(obj);

    if (objOut) {
        *objOut = &drawObjectInput.back();
    }

    return SFML_ERROR_OK;
}

SfmlError SfmlCoreWindow::DeleteDrawnObject(const SFML_OBJECT* obj)
{
    std::unique_lock<std::mutex> mlock(drawObjectSync);

    if (!obj) {
        return SFML_ERROR_INPUT;
    }

    

    return SFML_ERROR_OK;
}

sf::Color SfmlCoreWindow::convertHexToSfmlColor(unsigned long color)
{
    sf::Color out;

    // default is opaque
    out.a = 0xff;

    out.r = (color >> 16) & 0xff;
    out.g = (color >> 8) & 0xff;
    out.b = color & 0xff;

    if (color > 0x00ffffff) {
        out.a = (color >> 24) & 0xff;
    } 

    return out;
}