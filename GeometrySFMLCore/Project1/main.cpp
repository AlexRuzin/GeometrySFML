#if defined(_WIN32)
#define NOMINMAX
#include <Windows.h>
#endif //_WIN32

#define _USE_MATH_DEFINES
#include <cmath>

#include "sfml.h"
#include "defines.h"
#include "debug.h"

int main(void)
{
    SfmlCoreWindow sfmlWindow(SFML_WINDOW_SIZE_X, SFML_WINDOW_SIZE_Y, SFML_WINDOW_NAME);

    SfmlError sfmlError = sfmlWindow.StartWindowThread();
    if (sfmlError) {
        LOG_ERROR("Failure starting SFML window: " + std::to_string(sfmlError));
        return sfmlError;
    }

#if 0
#define ITER_LOOP 1000
    while (true) {
        Sleep(ITER_LOOP);

        SFML_OBJECT* circleObj = nullptr;
        sfmlWindow.DrawCircle(350, 250, 100, 0xFF00FF, &circleObj);        

        Sleep(ITER_LOOP);

        SFML_OBJECT *lineObj = nullptr;
        sfmlWindow.DrawLine(100, 100, 300, 300, 0xffffff, 0xffffff, &lineObj);
        Sleep(ITER_LOOP);

        sfmlWindow.DeleteDrawnObject(circleObj);
        sfmlWindow.DeleteDrawnObject(lineObj);
    }
#endif

    Sleep(1000);

    SFML_OBJECT* circleObj = nullptr;
    static const float circleCenterX = SFML_WINDOW_SIZE_X / 2.f;
    static const float circleCenterY = SFML_WINDOW_SIZE_X / 2.f;
    sfmlWindow.DrawCircle(
        circleCenterX - SFML_CIRCLE_RADIUS,
        circleCenterY - SFML_CIRCLE_RADIUS,
        SFML_CIRCLE_RADIUS, 
        SFML_CIRCLE_COLOR,
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
            nullptr
        );

        Sleep(100);
    }

    Sleep(INFINITE);

    return 0;
}