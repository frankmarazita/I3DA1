#include "projectile.h"

Projectile::Projectile(vec2f location, vec2fPolar polar, bool isBoat, int boatNum)
{
    this->location = location;
    this->polar = polar;
    this->isBoat = isBoat;
    this->boatNum = boatNum;

    r0 = location;
    v0 = polarToCartesian(polar);
    r = location;
    v = v0;
}

void Projectile::draw(Wave *wave)
{
    int numSegments = 100;

    // Draw Projectile
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegments; i++)
    {
        float theta = 2.0 * M_PI * float(i) / float(numSegments);
        float x1 = cos(theta) * radius;
        float y1 = sin(theta) * radius;
        glVertex2f(x1 + r.x, y1 + r.y);
    }
    glEnd();

    // Draw Trajectory Curve
    vec2f rTemp = r;
    vec2f vTemp = v;
    float t = 0.01;
    // Wave intersection
    float y = wave->getYfromX(rTemp.x);
    glBegin(GL_LINE_STRIP);
    while (rTemp.y > y)
    {
        rTemp.x += vTemp.x * t;
        rTemp.y += vTemp.y * t;
        vTemp.y += g * t;
        glVertex2f(rTemp.x, rTemp.y);
        y = wave->getYfromX(rTemp.x);
    }
    glEnd();
}

void Projectile::updateProjectileState(float dt)
{
    // Update projectile values and location
    r.x += v.x * dt;
    r.y += v.y * dt;
    v.y += g * dt;

    location = {r.x, r.y};
}

vec2f Projectile::getLocation()
{
    return location;
}

bool Projectile::getCollision(float radius, vec2f location)
{
    float dx = this->location.x - location.x;
    float dy = this->location.y - location.y;
    float distance = sqrt(pow(dx, 2) + pow(dy, 2));
    // Check projectile collision with other based on radius
    if (distance < radius + this->radius)
        return true;
    return false;
}

bool Projectile::getIsBoat()
{
    // Return projectile creation object
    return isBoat;
}

int Projectile::getBoatNum()
{
    return boatNum;
}