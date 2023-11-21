#include "sfml.h"

#include "SFML/Graphics.hpp"

#include <string>
#include <thread>

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

    while (renderWindow->isOpen() && !windowThreadSync.IsSignaled()) {
        // Event processing
        sf::Event event;
        while (renderWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                renderWindow->close();
        }

        // Rendering
        renderWindow->clear(sf::Color::Black);
        renderWindow->draw(circle);
        renderWindow->draw(radiusLine);
        renderWindow->display();
    }

    renderWindow->close();
    delete renderWindow;
    renderWindow = nullptr;

    return;
}