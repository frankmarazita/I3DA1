#include "boat.h"

Boat::Boat(vec2f location, float boatDeg, float cannonDeg, int direction)
{
    this->location = location;
    this->boatDeg = boatDeg;
    this->projectilePolar.angle = cannonDeg;
    this->direction = direction;

    // Set boat colour based on initial direction
    if (direction == 0)
    {
        boatColour = 0;
        col = {1, 0, 0, 1};
    }
    else
    {
        boatColour = 1;
        col = {0, 0, 1, 1};
    }
}

void Boat::draw()
{
    // Translate, scale and rotate
    glTranslatef(location.x, location.y, 0.0);
    glScalef(scale, scale, 1.0);
    glRotatef(boatDeg, 0, 0, 1.0);

    glColor3f(col.r, col.g, col.b);

    glPushMatrix();
    // Draw Hull
    glBegin(GL_POLYGON);
    glVertex2f(-1.0, 0.25);
    glVertex2f(1.0, 0.25);
    glVertex2f(0.5, -0.25);
    glVertex2f(-0.5, -0.25);
    glEnd();
    // Draw Bridge
    glBegin(GL_POLYGON);
    glVertex2f(-0.25, 0.25);
    glVertex2f(0.25, 0.25);
    glVertex2f(0.25, 0.75);
    glVertex2f(-0.25, 0.75);
    glEnd();
    // Draw Cannon
    glPushMatrix();
    if (direction == 0)
        glTranslatef(0.5, 0.25, 0.0);
    else
        glTranslatef(-0.5, 0.25, 0.0);
    glRotatef(projectilePolar.angle, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(0, 0.05);
    glVertex2f(0.5, 0.05);
    glVertex2f(0.5, -0.05);
    glVertex2f(0, -0.05);
    glEnd();
    glPopMatrix();
    glPopMatrix();

    // Calculate projectile creation location based on boat location ,
    // rotation and cannon rotation
    float radBoat = degToRad(boatDeg);
    float radCannon = 0;

    vec2f cannonOrigin = {0, 0};
    vec2f projectileDestination = {0, 0};
    vec2f newCannonOrigin = {0, 0};
    vec2f newProjectileDestinationBoat = {0, 0};
    vec2f newProjectileDestinationCannon = {0, 0};

    // Determine relative cannon location based on direction
    if (direction == 0)
    {
        radCannon = degToRad(projectilePolar.angle);
        cannonOrigin = {0.5, 0.25};
        projectileDestination = {1, 0.25};
    }
    else
    {
        radCannon = degToRad(projectilePolar.angle + 180);
        cannonOrigin = {-0.5, 0.25};
        projectileDestination = {-1, 0.25};
    }

    // Calculate end of cannon coordinate based boat and cannon rotations
    newCannonOrigin.x = cos(radBoat) * (cannonOrigin.x) - sin(radBoat) *
                                                              (cannonOrigin.y);
    newCannonOrigin.y = sin(radBoat) * (cannonOrigin.x) + cos(radBoat) *
                                                              (cannonOrigin.y);
    newProjectileDestinationBoat.x =
        cos(radBoat) * (projectileDestination.x) -
        sin(radBoat) * (projectileDestination.y);
    newProjectileDestinationBoat.y =
        sin(radBoat) * (projectileDestination.x) +
        cos(radBoat) * (projectileDestination.y);
    newProjectileDestinationCannon.x =
        cos(radCannon) * (newProjectileDestinationBoat.x - newCannonOrigin.x) -
        sin(radCannon) * (newProjectileDestinationBoat.y - newCannonOrigin.y) +
        newCannonOrigin.x;
    newProjectileDestinationCannon.y =
        sin(radCannon) * (newProjectileDestinationBoat.x - newCannonOrigin.x) +
        cos(radCannon) * (newProjectileDestinationBoat.y - newCannonOrigin.y) +
        newCannonOrigin.y;

    //Scale and translate location
    newProjectileDestinationCannon.x *= scale;
    newProjectileDestinationCannon.y *= scale;
    newProjectileDestinationCannon.x += location.x;
    newProjectileDestinationCannon.y += location.y;

    // Set the cannon vec2f location
    cannonLocation = newProjectileDestinationCannon;
}

void Boat::drawProjectile(Wave *wave)
{
    for (std::list<Projectile *>::iterator p = projectiles.begin();
         p != projectiles.end(); ++p)
    {
        (*p)->draw(wave);
    }
}

void Boat::drawDefence(Wave *wave)
{
    for (std::list<Defence *>::iterator d = defences.begin();
         d != defences.end(); ++d)
    {
        (*d)->draw(wave);
    }
}

void Boat::drawHealth()
{
    float x = -0.95 + 0.05 * health;

    // Draw health bar for specific boat
    glPushMatrix();
    glColor3f(col.r, col.g, col.b);
    glBegin(GL_POLYGON);
    if (boatColour == 0)
    {
        glVertex2f(-0.95, 0.95);
        glVertex2f(-0.95, 0.92);
        glVertex2f(x, 0.92);
        glVertex2f(x, 0.95);
    }
    else
    {
        glVertex2f(-0.95, 0.91);
        glVertex2f(-0.95, 0.88);
        glVertex2f(x, 0.88);
        glVertex2f(x, 0.91);
    }
    glEnd();
    glPopMatrix();
}

void Boat::moveLeft(float amount)
{
    // Movement restricted to bounding box
    if ((location.x - scale - amount >= -1 && boatColour == 0) ||
        (location.x - scale - amount >= 0.25 && boatColour == 1))
    {
        location.x -= amount;
        // Change boat cannon direction
        if (projectilePolar.angle < 90 && direction == 0)
            projectilePolar.angle = 180 - projectilePolar.angle;
        else if (projectilePolar.angle > 90 && direction == 0)
            projectilePolar.angle = 180 - projectilePolar.angle;
        // Set direction
        direction = 1;
    }
}

void Boat::moveRight(float amount)
{
    // Movement restricted to bounding box
    if ((location.x + scale + amount <= -0.25 && boatColour == 0) ||
        (location.x + scale + amount <= 1 && boatColour == 1))
    {
        location.x += amount;
        // Change boat cannon direction
        if (projectilePolar.angle > 90 && direction == 1)
            projectilePolar.angle = 180 - projectilePolar.angle;
        else if (projectilePolar.angle < 90 && direction == 1)
            projectilePolar.angle = 180 - projectilePolar.angle;
        // Set direction
        direction = 0;
    }
}

void Boat::cannonLeft(float amount)
{
    // Cannon rotation restrictions and direction accountability
    if (direction == 0)
    {
        if (projectilePolar.angle + amount <= 180)
            projectilePolar.angle += amount;
    }
    else if (projectilePolar.angle - amount >= 0)
        projectilePolar.angle -= amount;
}

void Boat::cannonRight(float amount)
{
    // Cannon rotation restrictions and direction accountability
    if (direction == 0)
    {
        if (projectilePolar.angle - amount >= 0)
            projectilePolar.angle -= amount;
    }
    else if (projectilePolar.angle + amount <= 180)
        projectilePolar.angle += amount;
}

void Boat::shoot()
{
    // Check for projectile shoot cooldown
    if (glutGet(GLUT_ELAPSED_TIME) - shootTime >= COOLDOWN)
    {
        shootTime = glutGet(GLUT_ELAPSED_TIME);
        // Create a new projectile
        vec2fPolar cannonProjectilePolar =
            {projectilePolar.magnitude, projectilePolar.angle + boatDeg};
        Projectile *projectile =
            new Projectile(cannonLocation, cannonProjectilePolar, true, boatColour);
        projectiles.push_back(projectile);
        projectileCount++;
    }
}

void Boat::defence()
{
    // Check for defence shoot cooldown
    if (glutGet(GLUT_ELAPSED_TIME) - defenceTime >= COOLDOWN)
    {
        defenceTime = glutGet(GLUT_ELAPSED_TIME);
        // Create a new defence
        vec2fPolar cannonProjectilePolar =
            {projectilePolar.magnitude, projectilePolar.angle + boatDeg};
        Defence *defence =
            new Defence(cannonLocation, cannonProjectilePolar, glutGet(GLUT_ELAPSED_TIME));
        defences.push_back(defence);
        defenceCount++;
    }
}

void Boat::updateProjectile(float dt)
{
    std::list<Projectile *>::iterator it;
    // Update each projectile
    for (std::list<Projectile *>::iterator p = projectiles.begin();
         p != projectiles.end(); ++p)
    {
        (*p)->updateProjectileState(dt);
    }
}

void Boat::updateDefence(float dt)
{
    std::list<Defence *>::iterator it;
    // Update each defence
    for (std::list<Defence *>::iterator d = defences.begin();
         d != defences.end(); ++d)
    {
        (*d)->updateDefenceState(dt);
    }
    // Check each defence for decay removal
    for (std::list<Defence *>::iterator d = defences.begin();
         d != defences.end(); ++d)
    {
        if (glutGet(GLUT_ELAPSED_TIME) - (*d)->getCreationTime() >= DEFENCE_DECAY)
        {
            defences.erase(d);
            break;
        }
    }
}

vec2f Boat::getLocation()
{
    return location;
}

float Boat::getCannonDeg()
{
    return projectilePolar.angle;
}

float Boat::getScale()
{
    return scale;
}

vec2f Boat::getCannonLocation()
{
    return cannonLocation;
}

float Boat::getBoatDeg()
{
    return boatDeg;
}

void Boat::setLocation(vec2f location)
{
    this->location = location;
}

void Boat::setBoatDeg(float boatDeg)
{
    this->boatDeg = boatDeg;
}

void Boat::setScale(float scale)
{
    this->scale = scale;
}

bool Boat::getProjectileExists()
{
    return projectileCount > 0;
}

bool Boat::getDefenceExists()
{
    return defenceCount > 0;
}

std::list<Projectile *> *Boat::getProjectiles()
{
    return &projectiles;
}

std::list<Defence *> *Boat::getDefences()
{
    return &defences;
}

void Boat::removeProjectile()
{
    projectileCount--;
}

void Boat::removeDefence()
{
    defenceCount--;
}

bool Boat::damage()
{
    if (health > 0)
        health--;
    if (health == 0)
        return true;
    return false;
}

bool Boat::getAlive()
{
    return health > 0;
}