#ifndef BOAT_H
#define BOAT_H

#include "vec2f.h"
#include "vec2fPolar.h"
#include "vec3f.h"
#include "colour.h"
#include "functions.h"
#include "projectile.h"
#include "defence.h"
#include "wave.h"

#include <cmath>
#include <list>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Boat
{
public:
    Boat(vec2f location, float boatDeg, float cannonDeg, int direction);

    void draw();
    void drawProjectile(Wave *wave);
    void drawDefence(Wave *wave);
    void drawHealth();

    void moveLeft(float amount);
    void moveRight(float amount);
    void cannonLeft(float amount);
    void cannonRight(float amount);

    void shoot();
    void defence();
    void updateProjectile(float dt);
    void updateDefence(float dt);

    vec2f getLocation();
    float getCannonDeg();
    float getScale();
    vec2f getCannonLocation();
    float getBoatDeg();

    void setLocation(vec2f location);
    void setBoatDeg(float boatDeg);
    void setScale(float scale);

    bool getProjectileExists();
    bool getDefenceExists();
    void removeProjectile();
    void removeDefence();
    bool damage();
    bool getAlive();

    std::list<Projectile *> *getProjectiles();
    std::list<Defence *> *getDefences();

private:
    vec2f location;
    float boatDeg;
    int direction;
    int boatColour;
    vec2f cannonLocation;
    vec2fPolar projectilePolar = {0.5, 90};

    float scale = 1;
    int health = 10;

    const float COOLDOWN = 1000;
    const float DEFENCE_DECAY = 3000;
    float shootTime = -1;
    float defenceTime = -1;
    int projectileCount = 0;
    int defenceCount = 0;
    std::list<Projectile *> projectiles;
    std::list<Defence *> defences;

    colour col;
};

#endif // BOAT_H
