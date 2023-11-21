#if defined(_WIN32)
#define NOMINMAX
#include <Windows.h>
#endif //_WIN32

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

    Sleep(2000);

    SFML_OBJECT *circleObj = nullptr;
    sfmlWindow.DrawCircle(350,250, 100, 0xFF00FF, &circleObj);

    Sleep(INFINITE);

    return 0;
}