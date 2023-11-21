#if defined(_WIN32)
#define NOMINMAX
#include <Windows.h>
#endif //_WIN32

#include "sfml.h"
#include "defines.h"

int main(void)
{
    SfmlCoreWindow sfmlWindow(SFML_WINDOW_SIZE_X, SFML_WINDOW_SIZE_Y, SFML_WINDOW_NAME);

    return 0;
}