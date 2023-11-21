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

    Sleep(INFINITE);

    return 0;
}