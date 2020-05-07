#include "island.h"

Island::Island()
{
}

void Island::draw()
{
    // Draw Island
    glColor3f(col.r, col.g, col.b);

    glPushMatrix();

    // Draw Island Base
    glBegin(GL_POLYGON);
    glVertex2f(-0.25, 0.25);
    glVertex2f(0.25, 0.25);
    glVertex2f(0.25, -1.0);
    glVertex2f(-0.25, -1.0);
    glEnd();
    // Draw Island Cannon
    glPushMatrix();
    glTranslatef(0.0, 0.25, 0.0);
    glRotatef(projectilePolar.angle, 0, 0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(0, 0.03);
    glVertex2f(0, -0.03);
    glVertex2f(0.25, -0.03);
    glVertex2f(0.25, 0.03);
    glEnd();
    glPopMatrix();

    glPopMatrix();

    // Set cannon cartesian location based on angle
    cannonLocation.x = 0.25 * cos(degToRad(projectilePolar.angle));
    cannonLocation.y = 0.25 * sin(degToRad(projectilePolar.angle)) + 0.25;
}

void Island::drawProjectile(Wave *wave)
{
    // Draw each of the wave's projectiles
    for (std::list<Projectile *>::iterator p = projectiles.begin();
         p != projectiles.end(); ++p)
    {
        (*p)->draw(wave);
    }
}

void Island::drawHealth()
{
    float x = -0.95 + 0.005 * health;
    // Draw health bar of the island
    glPushMatrix();
    glColor3f(col.r, col.g, col.b);
    glBegin(GL_POLYGON);
    glVertex2f(-0.95, 0.87);
    glVertex2f(-0.95, 0.84);
    glVertex2f(x, 0.84);
    glVertex2f(x, 0.87);
    glEnd();
    glPopMatrix();
}

void Island::cannonLeft(float amount)
{
    // Cannon movement restrictions
    if (projectilePolar.angle + amount <= 180)
    {
        projectilePolar.angle += amount;
        if (projectilePolar.angle >= 360)
            projectilePolar.angle -= 360;
    }
}

void Island::cannonRight(float amount)
{
    // Cannon movement restrictions
    if (projectilePolar.angle - amount >= 0)
    {
        projectilePolar.angle -= amount;
        if (projectilePolar.angle < 0)
            projectilePolar.angle += 360;
    }
}

void Island::shoot()
{
    // Check for projectile shoot cooldown
    if (glutGet(GLUT_ELAPSED_TIME) - shootTime >= cooldownTime)
    {
        shootTime = glutGet(GLUT_ELAPSED_TIME);
        // Create a new projectile
        Projectile *projectile = new Projectile(cannonLocation,
                                                projectilePolar, false, 0);
        projectiles.push_back(projectile);
        projectileCount++;
    }
}

void Island::updateProjectile(float dt)
{
    std::list<Projectile *>::iterator it;
    // Update each projectile
    for (std::list<Projectile *>::iterator p = projectiles.begin();
         p != projectiles.end(); ++p)
    {
        (*p)->updateProjectileState(dt);
    }
}

float Island::getCannonDeg()
{
    return projectilePolar.angle;
}

vec2f Island::getCannonLocation()
{
    return cannonLocation;
}

bool Island::getProjectileExists()
{
    return projectileCount > 0;
}

std::list<Projectile *> *Island::getProjectiles()
{
    return &projectiles;
}

void Island::removeProjectile()
{
    projectileCount--;
}

bool Island::collision(vec2f location)
{
    bool collision = false;
    // Check if a cartesian location is in collision with the island
    if (location.x >= -0.25 && location.x <= 0.25)
    {
        if (location.y <= 0.25)
        {
            collision = true;
        }
    }

    return collision;
}

bool Island::damage()
{
    if (health > 0)
        health--;
    if (health == 0)
        return true;
    return false;
}

bool Island::getAlive()
{
    return health > 0;
}