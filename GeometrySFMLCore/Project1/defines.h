#pragma once

#define SFML_WINDOW_SIZE_X                              600
#define SFML_WINDOW_SIZE_Y                              800
#define SFML_WINDOW_NAME                                "SacredGeometry"

#define SFML_ANTI_ALIASING

#if defined(SFML_ANTI_ALIASING)
#define SFML_ANTI_ALIASING_VAL                          8
#endif //SFML_ANTI_ALIASING

#define SFML_FRAME_RATE                                 60