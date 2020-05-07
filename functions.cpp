#include "functions.h"

float radToDeg(float rad)
{
    return rad * 180.0 / M_PI;
}

float degToRad(float deg)
{
    return deg * M_PI / 180.0;
}

float gradToRad(float grad)
{
    return atan(grad);
}

float calcGrad(float x1, float y1, float x2, float y2)
{
    return (y2 - y1) / (x2 - x1);
}

vec2f polarToCartesian(vec2fPolar polar)
{
    vec2f cart;
    cart.x = cos(degToRad(polar.angle)) * polar.magnitude;
    cart.y = sin(degToRad(polar.angle)) * polar.magnitude;
    return cart;
}

vec2fPolar cartesianToPolar(vec2f cart)
{
    vec2fPolar polar;
    polar.magnitude = sqrt(pow(cart.x, 2) + pow(cart.y, 2));
    polar.angle = radToDeg(gradToRad(cart.y / cart.x));
    return polar;
}