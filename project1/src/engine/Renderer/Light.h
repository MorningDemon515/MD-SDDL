#ifndef LIGHT_H
#define LIGHT_H

#include "../Math/Math.h"

typedef struct Light
{
    int type; // 0: parallel 1: point 2: spot

    alignas(16) Vector position;
    alignas(16) Vector direction;
    alignas(16) Vector color;

    alignas(4) float constant;
    alignas(4) float linear;
    alignas(4) float quadratic;

    alignas(4) float cutOff;
    alignas(4) float outerCutOff;
    alignas(4) float strength;
} Light;

#endif