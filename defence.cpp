#include "defence.h"

Defence::Defence(vec2f location, vec2fPolar polar, float creationTime)
{
    this->location = location;
    this->polar = polar;
    this->creationTime = creationTime;

    r0 = location;
    v0 = polarToCartesian(polar);
    r = location;
    v = v0;
}

void Defence::draw(Wave *wave)
{
    // Draw defence
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslated(r.x, r.y, 0.0);
    glutWireSphere(radius, 8, 8);
    glPopMatrix();
}

void Defence::updateDefenceState(float dt)
{
    // Update defence values and location
    r.x += v.x * dt;
    r.y += v.y * dt;
    v.y += g * dt;

    location = {r.x, r.y};
}

vec2f Defence::getLocation()
{
    return location;
}

bool Defence::getCollision(float radius, vec2f location)
{
    float dx = this->location.x - location.x;
    float dy = this->location.y - location.y;
    float distance = sqrt(pow(dx, 2) + pow(dy, 2));
    // Check defence collision with other based on radius
    if (distance < radius + this->radius)
        return true;
    return false;
}

float Defence::getCreationTime()
{
    return creationTime;
}

float Defence::getRadius()
{
    return radius;
}

void Defence::increaseRadius()
{
    // Grow defence size based on time
    radius = 0.00005 * (glutGet(GLUT_ELAPSED_TIME) - creationTime);
}