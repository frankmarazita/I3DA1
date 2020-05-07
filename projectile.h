#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "vec2f.h"
#include "vec2fPolar.h"
#include "vec3f.h"
#include "colour.h"
#include "functions.h"
#include "wave.h"

#include <cmath>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Projectile
{
public:
    Projectile(vec2f location, vec2fPolar polar, bool isBoat, int boatNum);

    void draw(Wave *wave);

    void updateProjectileState(float dt);

    vec2f getLocation();

    bool getCollision(float radius, vec2f location);
    bool getIsBoat();
    int getBoatNum();

private:
    vec2f location;
    vec2fPolar polar;
    bool isBoat;
    int boatNum;

    vec2f r0 = {0, 0}; // Initial position
    vec2f v0 = {0, 0}; // Initial velocity
    vec2f r = {0, 0};  // Drawn position
    vec2f v = {0, 0};  // Drawn velocity

    float g = -0.25;

    float radius = 0.01;
};

#endif // PROJECTILE_H