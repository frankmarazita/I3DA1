#ifndef ISLAND_H
#define ISLAND_H

#include "vec2f.h"
#include "vec2fPolar.h"
#include "vec3f.h"
#include "colour.h"
#include "functions.h"
#include "projectile.h"
#include "wave.h"

#include <cmath>
#include <list>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Island
{
public:
    Island();

    void draw();
    void drawProjectile(Wave *wave);
    void drawHealth();

    void cannonLeft(float amount);
    void cannonRight(float amount);

    void shoot();
    void updateProjectile(float dt);

    float getCannonDeg();
    vec2f getCannonLocation();
    bool getProjectileExists();
    void removeProjectile();
    bool collision(vec2f location);
    bool damage();
    bool getAlive();

    std::list<Projectile *> *getProjectiles();

private:
    vec2f cannonLocation = {0, 0};
    vec2fPolar projectilePolar = {0.4, 90};

    int health = 100;

    const float cooldownTime = 1000;
    float shootTime = -1;
    int projectileCount = 0;
    std::list<Projectile *> projectiles;

    colour col = {1.0, 1.0, 0.0, 1.0};
};

#endif // ISLAND_H