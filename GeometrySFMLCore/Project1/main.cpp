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
float negateFloat(float n);
unsigned long increaseColorGradient(unsigned long color, uint8_t increment);
unsigned long iterateRgbThroughLightGradient(unsigned long color, float increment);

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
    const float a = ((radius * radius) - (radius * radius) + (distance * distance)) / (2.0f * distance);

    // Point 2 coordinates
    const float x2 = originator.GetPosX() + (dx * a / distance);
    const float y2 = originator.GetPosY() + (dy * a / distance);

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
    //const float angle = (float)M_PI / 3.0f; //60 degrees

    std::vector<circleElement *> circleElements;  
    uint16_t firstIntersect = 0;
    circleElement *newElement = nullptr;

    // Determines initial positions
    for (uint16_t roundCount = 0; roundCount < SFML_FLOWER_COUNT; roundCount++) {
        const float offset = !roundCount ? 0 : 360.f / (roundCount * 6);

        if (roundCount == 0) {
            newElement = new circleElement{
                sfmlWindow,
                0,
                centerX, centerY,
                SFML_FLOWER_RADIUS,
                SFML_FLOWER_CIRCLE_STARTING_BACKGROUND,
                SFML_FLOWER_CIRCLE_BOUNDARY_COLOR,
                SFML_FLOWER_CIRCLE_BOUNDARY_THICKNESS
            };
            circleElements.push_back(newElement);
        }

        for (uint16_t i = 0; i < roundCount * 6; i++) {
            if (roundCount == 1 && i == 0) {
                newElement = new circleElement{
                    sfmlWindow,
                    roundCount,
                    centerX + SFML_FLOWER_RADIUS * std::cosf(0.f),
                    centerY + SFML_FLOWER_RADIUS * std::sinf(0.f),
                    SFML_FLOWER_RADIUS,
                    SFML_FLOWER_CIRCLE_STARTING_BACKGROUND,
                    SFML_FLOWER_CIRCLE_BOUNDARY_COLOR,
                    SFML_FLOWER_CIRCLE_BOUNDARY_THICKNESS
                };
            } else {
                float intersectX, intersectY;
                getCircleIntersection(
                    *circleElements.at(circleElements.size() - 1), *circleElements.at(firstIntersect),
                    SFML_FLOWER_RADIUS,
                    intersectX, intersectY
                );
                newElement = new circleElement{
                    sfmlWindow,
                    roundCount,
                    intersectX,
                    intersectY,
                    SFML_FLOWER_RADIUS,
                    SFML_FLOWER_CIRCLE_STARTING_BACKGROUND,
                    SFML_FLOWER_CIRCLE_BOUNDARY_COLOR,
                    SFML_FLOWER_CIRCLE_BOUNDARY_THICKNESS
                };
            }

            circleElements.push_back(newElement);

            const float vertexCalc = std::fmodf(newElement->GetTheta() + offset, 60.f) > 1;

            if (i == roundCount * 6 - 1) {
                firstIntersect++;
            }
            else if (vertexCalc) {
                firstIntersect++;
            }
        }
    }

    Sleep(100);


    float visualRadius = SFML_FLOWER_RADIUS;
    float visualRadiusInc = SFML_FLOWER_VISUAL_RADIUS_INCREMENT;
    uint16_t pauseIterations = 0;
    const uint16_t pauseIterationsMax = SFML_FLOWER_PAUSE_AT_INTERSECTION;
    const uint16_t timerSleep = SFML_FLOWER_PRIMARY_INTERVAL_MS;

    unsigned long currColor = SFML_FLOWER_CIRCLE_STARTING_BACKGROUND;
    while (true)
    {
        // Compute vector of gradients based on round
        // Compute vector of radius to round
        std::vector<unsigned long> gradientsPerRound;
        std::vector<float> radiusPerRound;

        for (uint16_t i = 0; i < SFML_FLOWER_COUNT; i++) {
            gradientsPerRound.push_back(iterateRgbThroughLightGradient(currColor, 
                SFML_FLOWER_COLOR_GRADIENT_INCREMENT == 0.f ? 0.f : i * SFML_FLOWER_COLOR_GRADIENT_INCREMENT));

            radiusPerRound.push_back(SFML_FLOWER_RADIUS_PER_ROUND_SCALE != 1.f 
                ? visualRadius + (i * SFML_FLOWER_RADIUS_PER_ROUND_SCALE) : visualRadius);
        }

        // Draw circles
        for (std::vector<circleElement*>::const_iterator i = circleElements.begin(); i != circleElements.end(); i++) { 
            (*i)->SetBackgroundColor(gradientsPerRound.at((*i)->GetRound()));
            (*i)->SetVisualRadius(radiusPerRound.at((*i)->GetRound()));

            (*i)->DrawCircle();
        }

        sfmlWindow.SignalDraw();

        //currColor = increaseColorGradient(currColor, 5);
        currColor = iterateRgbThroughLightGradient(currColor, SFML_FLOWER_COLOR_GRADIENT_INCREMENT); // Convert to HSL and preserve alpha

        if (circleElements.back()->GetVisualRadius() == SFML_FLOWER_RADIUS) {            
            pauseIterations++;
            if (pauseIterations == pauseIterationsMax) {
                pauseIterations = 0;
            } else {
                Sleep(timerSleep);
                continue;
            }
        }        

        visualRadius += SFML_FLOWER_VISUAL_RADIUS_INCREMENT ? visualRadiusInc : SFML_FLOWER_VISUAL_RADIUS_INCREMENT;
        if (visualRadius >= SFML_FLOWER_VISUAL_RADIUS_MAX || visualRadius <= SFML_FLOWER_VISUAL_RADIUS_MIN) {
            visualRadiusInc = negateFloat(visualRadiusInc);
        }     

        Sleep(timerSleep);
    }
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
        const float angle = 2.0f * (float)M_PI / (float)SFML_CIRCLE_POINT_COUNT * (float)pointCount;
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
        const float angle = 2.0f * M_PI / SFML_CIRCLE_POINT_COUNT * (float)pointCount;
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

//https://gist.github.com/ciembor/1494530
static HSL convertRgbToHsl(float r, float g, float b)
{
    HSL hsl;

    // Normalize RGB values
    float normR = r / 255.0;
    float normG = g / 255.0;
    float normB = b / 255.0;

    // Find the maximum and minimum values
    float maxVal = std::fmax(normR, std::fmax(normG, normB));
    float minVal = std::fmin(normR, std::fmin(normG, normB));

    // Calculate lightness
    hsl.l = (maxVal + minVal) / 2.0;

    // Check if the color is grayscale
    if (maxVal == minVal) {
        hsl.h = 0.0;
        hsl.s = 0.0;
    }
    else {
        // Calculate saturation
        if (hsl.l < 0.5) {
            hsl.s = (maxVal - minVal) / (maxVal + minVal);
        }
        else {
            hsl.s = (maxVal - minVal) / (2.0 - maxVal - minVal);
        }

        // Calculate hue
        float delta = maxVal - minVal;
        if (maxVal == normR) {
            hsl.h = (normG - normB) / delta;
        }
        else if (maxVal == normG) {
            hsl.h = 2.0 + (normB - normR) / delta;
        }
        else {
            hsl.h = 4.0 + (normR - normG) / delta;
        }

        hsl.h *= 60.0;
        if (hsl.h < 0.0) {
            hsl.h += 360.0;
        }
    }

    return hsl;
}

//https://gist.github.com/ciembor/1494530
float converHue2rgb(float v1, float v2, float vH)
{
    if (vH < 0)
        vH += 1;

    if (vH > 1)
        vH -= 1;

    if ((6 * vH) < 1)
        return (v1 + (v2 - v1) * 6 * vH);

    if ((2 * vH) < 1)
        return v2;

    if ((3 * vH) < 2)
        return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

    return v1;

}

//https://gist.github.com/ciembor/1494530
//https://www.programmingalgorithms.com/algorithm/hsl-to-rgb/c/
unsigned long convertHsl2rgb(HSL hsl) {
    uint8_t r, g, b;

    if (hsl.s == 0)
    {
        r = g = b = (unsigned char)(hsl.l * 255);
    }
    else
    {
        float v1, v2;
        float hue = (float)hsl.h / 360;

        v2 = (hsl.l < 0.5) ? (hsl.l * (1 + hsl.s)) : ((hsl.l + hsl.s) - (hsl.l * hsl.s));
        v1 = 2 * hsl.l - v2;

        r = (unsigned char)(255 * converHue2rgb(v1, v2, hue + (1.0f / 3)));
        g = (unsigned char)(255 * converHue2rgb(v1, v2, hue));
        b = (unsigned char)(255 * converHue2rgb(v1, v2, hue - (1.0f / 3)));
    }

    return  (0x00 << 24) | (r << 16) | (g << 8) | b;
}

unsigned long iterateRgbThroughLightGradient(unsigned long color, float increment)
{
    if (increment == 0.0f) {
        return color;
    }

    uint8_t a = (color >> 24) & 0xff;
    uint8_t r = (color >> 16) & 0xff;
    uint8_t g = (color >> 8) & 0xff;
    uint8_t b = color & 0xff;

    // Convert to HSL and increase hue by 1
    HSL hsl = convertRgbToHsl(r, g, b);
    hsl.h += hsl.h > 360.f ? -360.f + increment : increment;

    unsigned long rgbOut = (a << 24) | convertHsl2rgb(hsl);

    //system("cls");
    //printf("rgb color: 0x%08x\th: %f, s: %f, l: %f", rgbOut, hsl.h, hsl.s, hsl.l);

    return rgbOut;
}

static int8_t gradientInverse = 1;
unsigned long increaseColorGradient(unsigned long color, uint8_t increment)
{
    uint8_t a = (color >> 24) & 0xff;
    uint8_t r = (color >> 16) & 0xff;
    uint8_t g = (color >> 8) & 0xff;
    uint8_t b = color & 0xff;

    if (b  >= 0xff) {
        if (g >= 0xff) {
            if (r >= 0xff) {
                gradientInverse = -gradientInverse;
                r--;
            } else {
                r += increment * gradientInverse;
            }
        } else {
            g += increment * gradientInverse;
        }
    } else {
        b += increment * gradientInverse;
    }

    unsigned long out = (a << 24) | (r << 16) | (g << 8) | b;

   // system("cls");
    //printf("0x%08x", out);


    return out;
}

float negateFloat(float n)
{
    union { unsigned x; float y; } u;
    u.y = n;
    u.x ^= 0x80000000;
    return u.y;
}