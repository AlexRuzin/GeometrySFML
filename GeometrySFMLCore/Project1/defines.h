#pragma once

#define SFML_WINDOW_SIZE_X                              1200.f
#define SFML_WINDOW_SIZE_Y                              1200.f
#define SFML_WINDOW_NAME                                "SacredGeometry"

#define SFML_CIRCLE_RADIUS                              400.f
#define SFML_CIRCLE_COLOR                               0x111611

// Points on the circle's circumference
#define SFML_CIRCLE_POINT_RADIUS                        1.f
#define SFML_CIRCLE_POINT_COLOR                         0xfffffffa

#define SFML_CIRCLE_POINT_COUNT                         64

// Defines the point accuracy of the circle, the greater, the rounder the circle
#define SFML_CIRCLE_POINT_COUNT_ACCURACY                1000

#define SFML_ANTI_ALIASING

#if defined(SFML_ANTI_ALIASING)
#define SFML_ANTI_ALIASING_VAL                          10
#endif //SFML_ANTI_ALIASING

#define SFML_FRAME_RATE                                 60