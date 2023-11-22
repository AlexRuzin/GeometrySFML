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

#if defined(SFML_ANTI_ALIASING)
    sf::ContextSettings settings;
    settings.antialiasingLevel = SFML_ANTI_ALIASING_VAL;
#endif //SFML_ANTI_ALIASING

    renderWindow = new sf::RenderWindow(sf::VideoMode(winWidth, winHeight), winName, sf::Style::Default, settings);
    renderWindow->setFramerateLimit(SFML_FRAME_RATE);

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

    signalDoDraw.ResetSignal();

    while (renderWindow->isOpen() && !windowThreadSync.IsSignaled()) {

        // Event processing
        sf::Event event;
        while (renderWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                renderWindow->close();
            }
        }

        if (!signalDoDraw.IsSignaled()) {
            continue;
        }

        // Rendering
        //renderWindow->clear(convertHexToSfmlColor(0xfdf5e8));
        std::unique_lock<std::mutex> mlock(drawObjectLock);

        renderWindow->clear(winBackgroundColor);

        for (std::list<SFML_OBJECT>::iterator i = drawObjectInput.begin(); i != drawObjectInput.end(); i++) {  
            if (i->type == SFML_OBJ_CIRCLE) {
                sf::CircleShape circle(i->radius);

                circle.setFillColor(i->color1);
                  
                circle.setOutlineColor(i->color2);
                circle.setOutlineThickness(i->thickness);


                circle.setPosition(i->posX, i->posY);
                circle.setPointCount(SFML_CIRCLE_POINT_COUNT_ACCURACY);

                renderWindow->draw(circle);

            } else if (i->type == SFML_OBJ_LINE) {
                sf::VertexArray line(sf::Lines, 2);
                line[0].position = sf::Vector2f(i->posX, i->posY);
                line[1].position = sf::Vector2f(i->posX2, i->posY2);
                
                line[0].color = i->color1;
                line[1].color = i->color2;

                renderWindow->draw(line);
            }           
        }
        renderWindow->display();

        signalDoDraw.ResetSignal();
    }

    renderWindow->close();
    delete renderWindow;
    renderWindow = nullptr;

    return;
}

SfmlError SfmlCoreWindow::DrawCircle(
    float x, float y, float radius, 
    unsigned long backgroundColor, unsigned long edgeColor,
    float thickness,
    SFML_OBJECT **objOut)
{
    std::unique_lock<std::mutex> mlock(drawObjectLock);

    if (!renderWindow) {
        return SFML_ERROR_NOT_RUNNING;
    }

    SFML_OBJECT obj;
    obj.type = SFML_OBJ_CIRCLE;
    obj.posX = x - radius;
    obj.posY = y - radius;
    obj.radius = radius;
    obj.thickness = thickness;
    obj.color1 = convertHexToSfmlColor(backgroundColor);
    obj.color2 = convertHexToSfmlColor(edgeColor);

    drawObjectInput.push_back(obj);

    if (objOut) {
        *objOut = &drawObjectInput.back();
    }

    return SFML_ERROR_OK;
}

SfmlError SfmlCoreWindow::DrawLine(float x, float y, float x2, float y2,
    unsigned long color, unsigned long color2, SFML_OBJECT** objOut)
{
    std::unique_lock<std::mutex> mlock(drawObjectLock);

    if (!renderWindow) {
        return SFML_ERROR_NOT_RUNNING;
    }

    SFML_OBJECT obj;
    obj.type = SFML_OBJ_LINE;
    obj.posX = x;
    obj.posY = y;
    obj.posX2 = x2;
    obj.posY2 = y2;
    obj.color1 = convertHexToSfmlColor(color);
    obj.color2 = convertHexToSfmlColor (color2);

    drawObjectInput.push_back(obj);

    if (objOut) {
        *objOut = &drawObjectInput.back();
    }

    return SFML_ERROR_OK;
}

SfmlError SfmlCoreWindow::DeleteDrawnObject(const SFML_OBJECT* obj)
{
    std::unique_lock<std::mutex> mlock(drawObjectLock);

    if (!obj) {
        return SFML_ERROR_INPUT;
    }

    std::list<SFML_OBJECT>::iterator i;
    for (i = drawObjectInput.begin(); i != drawObjectInput.end(); i++) {
        if (&*i == obj) {
            break;
        }
    }

    if (i == drawObjectInput.end()) {
        return SFML_ERROR_OBJECT_DOES_NOT_EXIST;
    }

    drawObjectInput.erase(i);

    return SFML_ERROR_OK;
}

SfmlError SfmlCoreWindow::DeleteAllDrawnObject(void)
{
    std::unique_lock<std::mutex> mlock(drawObjectLock);

    drawObjectInput.clear();

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