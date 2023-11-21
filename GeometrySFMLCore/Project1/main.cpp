#if defined(_WIN32)
#define NOMINMAX
#include <Windows.h>
#endif //_WIN32

#define _USE_MATH_DEFINES
#include <cmath>

#include <string>
#include <vector>
#include <algorithm>
#include <random>

#include "main.h"
#include "sfml.h"
#include "defines.h"
#include "debug.h"

void drawCircleWithCrossSection(SfmlCoreWindow &sfmlWindow);
void drawCircleWithCrossSectionOuter(SfmlCoreWindow &sfmlWindow);
void drawVortexIter(SfmlCoreWindow &sfmlWindow);

void drawFlowerOfLife(SfmlCoreWindow &sfmlWindow);

bool getCircleIntersection(circleElement &originator, circleElement &intersected, float radius, float &outX, float &outY);
uint32_t genRandInt(uint32_t min, uint32_t max);
uint32_t sumOfDigits(uint32_t n, uint32_t base);

#define DEGREE_TO_RADIANS(x) (x * M_PI / 180)

int main(void)
{
    SfmlCoreWindow sfmlWindow(SFML_WINDOW_SIZE_X, SFML_WINDOW_SIZE_Y, SFML_WINDOW_NAME, SFML_WINDOW_BACKGROUND_COLOR);

    SfmlError sfmlError = sfmlWindow.StartWindowThread();
    if (sfmlError) {
        LOG_ERROR("Failure starting SFML window: " + std::to_string(sfmlError));
        return sfmlError;
    }

    Sleep(1000);

    //drawCircleWithCrossSection(sfmlWindow);
    //drawCircleWithCrossSectionOuter(sfmlWindow);
    //drawVortexIter(sfmlWindow);
    drawFlowerOfLife(sfmlWindow);

    Sleep(INFINITE);

    return 0;
}

void drawCircleWithCrossSectionOuter(SfmlCoreWindow &sfmlWindow)
{

}

bool getCircleIntersection(circleElement &originator, circleElement &intersected, float radius, float &outX, float &outY)
{
    // Horizontal/vertical distance between circle center
    const float dx = intersected.GetPosX() - originator.GetPosX();
    const float dy = intersected.GetPosY() - originator.GetPosY();

    // Distance between the two centers
    const float distance = std::hypotf(dx, dy);

    if (distance > (radius + radius)) {
        return false;
    }

    // Distance between 0 and 2
    const float a = ((radius * radius) - (radius * radius) + (distance * distance)) / (2.0 * distance);

    // Point 2 coordinates
    const float x2 = originator.GetPosX() + (dx * a / distance);
    const float y2 = originator.GetPosY() + (dx * a / distance);

    // Distance between point 2 to intersection
    const float h = std::sqrtf((radius * radius) - (a * a));

    // Intersection points
    const float rx = -dy * (h / distance);
    const float ry = dx * (h / distance);

    outX = x2 - rx;
    outY = y2 - ry;

    return true;
}

void drawFlowerOfLife(SfmlCoreWindow &sfmlWindow)
{
    const float angle = M_PI / 3; //60 degrees
    const float centerX = (SFML_WINDOW_SIZE_X / 2.f) - SFML_FLOWER_RADIUS;
    const float centerY = (SFML_WINDOW_SIZE_Y / 2.f) - SFML_FLOWER_RADIUS;

    std::vector<circleElement *> circleElements;    

    // central circle
    circleElements.push_back(new circleElement{        
        sfmlWindow,
        circleElements.size(),
        centerX, centerY,
        SFML_FLOWER_RADIUS,
        SFML_FLOWER_CIRCLE_STARTING_BACKGROUND,
        SFML_FLOWER_CIRCLE_BOUNDARY_COLOR,
        SFML_FLOWER_CIRCLE_BOUNDARY_THICKNESS
        });
    circleElements.back()->DrawCircle();

    for (uint16_t c = 0; c < 6; c++) {
        circleElements.push_back(new circleElement{
            sfmlWindow,
            circleElements.size(),
            circleElements.at(0)->GetPosX() + SFML_FLOWER_RADIUS * std::cosf(DEGREE_TO_RADIANS(60) * c),
            circleElements.at(0)->GetPosY() + SFML_FLOWER_RADIUS * std::sinf(DEGREE_TO_RADIANS(60) * c),
            SFML_FLOWER_RADIUS,
            SFML_FLOWER_CIRCLE_STARTING_BACKGROUND,
            SFML_FLOWER_CIRCLE_BOUNDARY_COLOR,
            SFML_FLOWER_CIRCLE_BOUNDARY_THICKNESS
        });
        circleElements.back()->DrawCircle();

        Sleep(10);
    }

#if 0

    for (uint16_t i = 0; i < 6; i++) {
        const float x = centerX + SFML_FLOWER_RADIUS * cos(DEGREE_TO_RADIANS(60) * i);
        const float y = centerY + SFML_FLOWER_RADIUS * sin(DEGREE_TO_RADIANS(60) * i);

        sfmlWindow.DrawCircle(
            x, y,
            SFML_FLOWER_RADIUS,
            SFML_FLOWER_CIRCLE_STARTING_BACKGROUND,
            SFML_FLOWER_CIRCLE_BOUNDARY_COLOR,
            SFML_FLOWER_CIRCLE_BOUNDARY_THICKNESS,
            nullptr);

        Sleep(100);
    }
#endif

#if 0
    float x, y;
    for (uint32_t layer = 1; layer <= SFML_FLOWER_LAYERS; layer++) {
        for (uint32_t i = 0; i < 6; i++) {
            x = centerX + layer * SFML_FLOWER_RADIUS * cos(i * angle);
            y = centerY + layer * SFML_FLOWER_RADIUS * sin(i * angle);

            sfmlWindow.DrawCircle(
                x, y, 
                SFML_FLOWER_RADIUS,
                0xffffff, 
                false,
                nullptr);

            for (uint32_t j = 1; j < layer; j++) {
                x += SFML_FLOWER_RADIUS * cos((i - 0.5f) * angle);
                y += SFML_FLOWER_RADIUS * sin((i - 0.5f) * angle);

                sfmlWindow.DrawCircle(
                    x, y,
                    SFML_FLOWER_RADIUS,
                    0xffffff,
                    false,
                    nullptr);
                Sleep(100);
            }
        }
    }
#endif
    

}

void drawVortexIter(SfmlCoreWindow &sfmlWindow)
{
    static const float circleCenterX = SFML_WINDOW_SIZE_X / 2.f;
    static const float circleCenterY = SFML_WINDOW_SIZE_X / 2.f;
    sfmlWindow.DrawCircle(
        circleCenterX - SFML_CIRCLE_RADIUS,
        circleCenterY - SFML_CIRCLE_RADIUS,
        SFML_CIRCLE_RADIUS,
        SFML_CIRCLE_COLOR,
        0xff000000,
        0.5f,
        nullptr);

    // x = x_center + r * cos(theta)
    // y = y_center + r * sin(theta)
    // Draw points
    for (uint32_t pointCount = 1; pointCount <= SFML_CIRCLE_POINT_COUNT; pointCount++) {
        const float angle = 2 * M_PI / SFML_CIRCLE_POINT_COUNT * (float)pointCount;
        const float x = circleCenterX + SFML_CIRCLE_RADIUS * sin(angle);
        const float y = circleCenterY + SFML_CIRCLE_RADIUS * cos(angle);

        sfmlWindow.DrawCircle(
            x, y,
            SFML_CIRCLE_POINT_RADIUS,
            SFML_CIRCLE_POINT_COLOR,
            0xff000000,
            0.5f,
            nullptr
        );        
    }

#if 1
    const unsigned long wheelLineColor = 0x01ffffff;
    for (uint32_t pointCount = 1; pointCount <= SFML_CIRCLE_POINT_COUNT; pointCount++) {
        const float angle = 2 * M_PI / SFML_CIRCLE_POINT_COUNT * pointCount;
        const float x = circleCenterX + SFML_CIRCLE_RADIUS * sin(angle);
        const float y = circleCenterY + SFML_CIRCLE_RADIUS * cos(angle);

        sfmlWindow.DrawCircle(
            x, y,
            SFML_CIRCLE_POINT_RADIUS,
            SFML_CIRCLE_POINT_COLOR,
            0xff000000,
            0.5f,
            nullptr
        );

        // Draw line from point(theta) to point(theta) + 1
        const float nextAngle = 2 * M_PI / SFML_CIRCLE_POINT_COUNT * ((uint32_t)pointCount + 1);
        const float x2 = circleCenterX + SFML_CIRCLE_RADIUS * sin(nextAngle);
        const float y2 = circleCenterY + SFML_CIRCLE_RADIUS * cos(nextAngle);
        sfmlWindow.DrawLine(
            x, y,
            x2, y2,
            wheelLineColor, wheelLineColor & 0xffffff,
            nullptr
        );

#if 1
        // Draw line from point(theta) to center
        sfmlWindow.DrawLine(
            x, y,
            circleCenterX, circleCenterY,
            wheelLineColor, wheelLineColor & 0xffffff,
            nullptr
        );
#endif

        Sleep(10);
    }
#endif

    const unsigned long lineColorDefault = 0x00fffffb;
    unsigned long lineColor = lineColorDefault;
    for (uint32_t circlePoint = 1; circlePoint <= SFML_CIRCLE_POINT_COUNT + 256; circlePoint++) {
        std::vector<uint32_t> knownPoints;
        uint32_t innerPoint = circlePoint;

        while (true) {
            const float angle1 = 2 * M_PI / SFML_CIRCLE_POINT_COUNT * innerPoint;
            const float x1 = circleCenterX + SFML_CIRCLE_RADIUS * sin(angle1);
            const float y1 = circleCenterY + SFML_CIRCLE_RADIUS * cos(angle1);

            innerPoint = innerPoint * 2 >= SFML_CIRCLE_POINT_COUNT + 1 ?
                sumOfDigits(innerPoint * 2, SFML_CIRCLE_POINT_COUNT + 1) : innerPoint * 2;

            if (std::find(knownPoints.begin(), knownPoints.end(), innerPoint) != knownPoints.end()) {
                break;
            }

            knownPoints.push_back(innerPoint);

            const float angle2 = 2 * M_PI / SFML_CIRCLE_POINT_COUNT * innerPoint;
            const float x2 = circleCenterX + SFML_CIRCLE_RADIUS * sin(angle2);
            const float y2 = circleCenterY + SFML_CIRCLE_RADIUS * cos(angle2);

            sfmlWindow.DrawLine(
                x1, y1,
                x2, y2,
                lineColor, lineColor,
                nullptr
            );
            
#if 0
            lineColor -= 100;

            if (lineColor == 0x00) {
                lineColor = lineColorDefault;
            }
#endif

            Sleep(1);
        }

#if 0
        lineColor -= 100;

        if (lineColor == 0x00) {
            lineColor = lineColorDefault;
        }
#endif
        //lineColor = genRandInt(0x101010, 0xffffff);
    }

#if 0
    // Draw vortex lines
    uint32_t currPoint = 3;
    //std::vector<float> knownPoints;
    for (uint32_t i = 0; i < 1000; i++) {
        const float angle1 = 2 * M_PI / SFML_CIRCLE_POINT_COUNT * currPoint;
        const float x1 = circleCenterX + SFML_CIRCLE_RADIUS * sin(angle1);
        const float y1 = circleCenterY + SFML_CIRCLE_RADIUS * cos(angle1);

        currPoint = currPoint * 2 >= SFML_CIRCLE_POINT_COUNT + 1 ?
            sumOfDigits(currPoint * 2, SFML_CIRCLE_POINT_COUNT + 1) : currPoint * 2;

#if 0
        if (std::find(knownPoints.begin(), knownPoints.end(), currPoint) != knownPoints.end()) {
            break;
        }

        knownPoints.push_back(currPoint);
#endif

        const float angle2 = 2 * M_PI / SFML_CIRCLE_POINT_COUNT * currPoint;
        const float x2 = circleCenterX + SFML_CIRCLE_RADIUS * sin(angle2);
        const float y2 = circleCenterY + SFML_CIRCLE_RADIUS * cos(angle2);

        sfmlWindow.DrawLine(
            x1, y1,
            x2, y2,
            0xffffff, 0xffffff,
            nullptr
        );
    }
#endif
}

void drawCircleWithCrossSection(SfmlCoreWindow &sfmlWindow)
{
    SFML_OBJECT* circleObj = nullptr;
    static const float circleCenterX = SFML_WINDOW_SIZE_X / 2.f;
    static const float circleCenterY = SFML_WINDOW_SIZE_X / 2.f;
    sfmlWindow.DrawCircle(
        circleCenterX - SFML_CIRCLE_RADIUS,
        circleCenterY - SFML_CIRCLE_RADIUS,
        SFML_CIRCLE_RADIUS,
        SFML_CIRCLE_COLOR,
        0xff000000,
        0.5f,
        &circleObj);

    // x = x_center + r * cos(theta)
    // y = y_center + r * sin(theta)
    for (uint32_t pointCount = 0; pointCount < SFML_CIRCLE_POINT_COUNT; pointCount++) {
        const float angle = 2 * M_PI / SFML_CIRCLE_POINT_COUNT * pointCount;
        const float x = circleCenterX + SFML_CIRCLE_RADIUS * cos(angle);
        const float y = circleCenterY + SFML_CIRCLE_RADIUS * sin(angle);

        sfmlWindow.DrawCircle(
            x, y,
            SFML_CIRCLE_POINT_RADIUS,
            SFML_CIRCLE_POINT_COLOR,
            0xff000000,
            0.5f,
            nullptr
        );

        // Draw line from point(theta) to point(theta) + 1
        const float nextAngle = 2 * M_PI / SFML_CIRCLE_POINT_COUNT * ((uint32_t)pointCount + 1);
        const float x2 = circleCenterX + SFML_CIRCLE_RADIUS * cos(nextAngle);
        const float y2 = circleCenterY + SFML_CIRCLE_RADIUS * sin(nextAngle);
        sfmlWindow.DrawLine(
            x, y,
            x2, y2,
            0xffffff, 0xffffff,
            nullptr
        );

        // Draw line from point(theta) to center
        sfmlWindow.DrawLine(
            x, y,
            circleCenterX, circleCenterY,
            0xffffff, 0xffffff,
            nullptr
        );

        Sleep(100);
    }
}

uint32_t genRandInt(uint32_t min, uint32_t max)
{
    std::mt19937 engine(std::random_device{}());
    std::uniform_int_distribution<uint32_t> distribution(min, max);
    return distribution(engine);
}

uint32_t sumOfDigits(uint32_t n, uint32_t base)
{
    uint32_t out = 0;
    while (n > 0) {
        out += n % base;
        n /= base;
    }

    return out;
}