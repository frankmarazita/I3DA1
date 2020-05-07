#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "vec2f.h"
#include "vec2fPolar.h"

#include <cmath>

float radToDeg(float rad);
float degToRad(float deg);
float gradToRad(float grad);
float calcGrad(float x1, float y1, float x2, float y2);
vec2f polarToCartesian(vec2fPolar polar);
vec2fPolar cartesianToPolar(vec2f cart);

#endif // FUNCTIONS_H