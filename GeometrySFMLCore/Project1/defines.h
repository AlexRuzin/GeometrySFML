#pragma once

#define SFML_WINDOW_SIZE_X                              1200.f
#define SFML_WINDOW_SIZE_Y                              1200.f
#define SFML_WINDOW_NAME                                "SacredGeometry"
#define SFML_WINDOW_BACKGROUND_COLOR                    0x00787878

//
// 3-6-9 Pattern
//

#define SFML_CIRCLE_RADIUS                              400.f
#define SFML_CIRCLE_COLOR                               0x111611

// Points on the circle's circumference
#define SFML_CIRCLE_POINT_RADIUS                        1.f
#define SFML_CIRCLE_POINT_COLOR                         0xfffffffa

#define SFML_CIRCLE_POINT_COUNT                         64

//
// Flower of life
//
#define SFML_FLOWER_RADIUS                              40.f
#define SFML_FLOWER_VISUAL_RADIUS_MIN                   SFML_FLOWER_RADIUS - (SFML_FLOWER_RADIUS / 2.0f)//52.f
#define SFML_FLOWER_VISUAL_RADIUS_MAX                   SFML_FLOWER_RADIUS + (SFML_FLOWER_RADIUS / 2.0f)//78.f
#define SFML_FLOWER_VISUAL_RADIUS_INCREMENT             0.1f

#define SFML_FLOWER_COUNT                               32
#define SFML_FLOWER_CIRCLE_BOUNDARY_THICKNESS           0.5f
#define SFML_FLOWER_CIRCLE_STARTING_BACKGROUND          0x1f7f00ff // alpha, r, g, b
#define SFML_FLOWER_CIRCLE_BOUNDARY_COLOR               0x71000000


//
// Visual definitions
//

// Defines the point accuracy of the circle, the greater, the rounder the circle
#define SFML_CIRCLE_POINT_COUNT_ACCURACY                64

#define SFML_ANTI_ALIASING

#if defined(SFML_ANTI_ALIASING)
#define SFML_ANTI_ALIASING_VAL                          8
#endif //SFML_ANTI_ALIASING

#define SFML_FRAME_RATE                                 60