// I3D Assignment 01 - Island Defence
// s3717437 - Frank Marazita

#include "vec2f.h"
#include "vec2fPolar.h"
#include "vec3f.h"
#include "colour.h"
#include "wave.h"
#include "boat.h"
#include "island.h"
#include "keyboard.h"

#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <list>
#include <string>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.141592653589793238463
#endif

const int milli = 1000;
const float windowSize = 1;

// Game objects
Wave *wave = new Wave(windowSize, 64, 0.25, 2 * M_PI, 0, 0);
vec2f boat1location = {-0.5, 0};
Boat *boat1 = new Boat(boat1location, 0, 45, 0);
vec2f boat2location = {0.5, 0};
Boat *boat2 = new Boat(boat2location, 0, 135, 1);
Island *island = new Island();
Keyboard *keyboard = new Keyboard();

typedef struct
{
    int windowHeight;
    int windowWidth;
    bool runnning;
    float startTime;
    int frames;
    float FPS;
    float FPSInterval;
    float lastFPSTime;
    bool wireframe;
    float keyPressTime;
} global_t;

global_t global = {600, 600, true, 0.0, 0, 0.0, 1, 0.0, false, -1};

// Application Functions
void myinit();
void myReshape(int w, int h);
void display();
void update();
void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void keyPress(unsigned char key);
void displayFPS();
void updateProjectiles(float dt);
void updateDefences(float dt);

// Draw funcitons
void drawAxis(float length);
void drawCircle(vec2f location, float r);

// Calc functions
float radToDeg(float rad);
float degToRad(float deg);
float gradToRad(float grad);
float calcGrad(float x1, float y1, float x2, float y2);

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    glutInitWindowSize(global.windowWidth, global.windowHeight);
    glutCreateWindow("Island Defence");

    myinit();

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);

    glutMainLoop();
}

// Application Functions

void myinit()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_FLAT);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Set global start time
    global.startTime = glutGet(GLUT_ELAPSED_TIME) / (float)milli;
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set global window dimensions
    global.windowWidth = w;
    global.windowHeight = h;
}

void display()
{
    // Error variable
    GLenum err;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw Axis
    drawAxis(windowSize);

    // Start Wireframe
    if (global.wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Draw Island
    island->draw();

    // Draw Boat 1
    glPushMatrix();
    boat1->setLocation({boat1->getLocation().x,
                        wave->getYfromX(boat1->getLocation().x)});
    boat1->setBoatDeg(wave->getGrad(boat1->getLocation().x));
    boat1->setScale(0.1);
    boat1->draw();
    glPopMatrix();
    // Draw Boat 1 Axis
    glPushMatrix();
    glTranslatef(boat1->getLocation().x, boat1->getLocation().y, 0.0);
    glRotatef(boat1->getBoatDeg(), 0, 0, 1.0);
    drawAxis(0.1);
    glPopMatrix();
    // Hitbox (Uncomment for visual)
    // drawCircle(boat1->getLocation(), boat1->getScale());

    // Draw Boat 2
    glPushMatrix();
    boat2->setLocation({boat2->getLocation().x,
                        wave->getYfromX(boat2->getLocation().x)});
    boat2->setBoatDeg(wave->getGrad(boat2->getLocation().x));
    boat2->setScale(0.1);
    boat2->draw();
    glPopMatrix();
    // Draw Boat 2 Axis
    glPushMatrix();
    glTranslatef(boat2->getLocation().x, boat2->getLocation().y, 0.0);
    glRotatef(boat2->getBoatDeg(), 0, 0, 1.0);
    drawAxis(0.1);
    glPopMatrix();
    // Hitbox (Uncomment for visual)
    // drawCircle(boat2->getLocation(), boat2->getScale());

    // Projectile creation location (Uncomment for visual)
    // drawCircle(island->getCannonLocation(), 0.01);
    // drawCircle(boat1->getCannonLocation(), 0.01);
    // drawCircle(boat2->getCannonLocation(), 0.01);

    // Draw Wave
    wave->draw();

    //Draw Health Bars
    island->drawHealth();
    boat1->drawHealth();
    boat2->drawHealth();

    // Draw Projectiles
    if (island->getProjectileExists())
        island->drawProjectile(wave);
    if (boat1->getProjectileExists())
        boat1->drawProjectile(wave);
    if (boat2->getProjectileExists())
        boat2->drawProjectile(wave);

    // Draw Defences
    if (boat1->getDefenceExists())
        boat1->drawDefence(wave);
    if (boat2->getDefenceExists())
        boat2->drawDefence(wave);

    // End Wireframe
    if (global.wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Game Over Message
    if (!global.runnning)
    {
        glPushMatrix();
        std::string text = "Game Over";
        int len = text.length();
        float dist = ((float)(len * 9) / global.windowWidth);
        glTranslatef(-dist, 0.0, 0.0);
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2f(0, 0);
        for (int i = 0; i < len; i++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
        glPopMatrix();
    }

    // FPS Counter
    displayFPS();

    glPopMatrix();

    glutSwapBuffers();

    global.frames++;

    while ((err = glGetError()) != GL_NO_ERROR)
        printf("display: %s\n", gluErrorString(err));
}

void update()
{
    // Last time only accessable in this function and value retained
    static float lastT = -1.0;
    // t: time since start (seconds) | dt: difference time from lastT (seconds)
    float t;
    float dt;

    t = glutGet(GLUT_ELAPSED_TIME) / (float)milli - global.startTime;

    if (lastT < 0.0)
    {
        lastT = t;
        return;
    }

    dt = t - lastT;
    // Move Wave
    if (wave->getAnimate())
        wave->moveWave(glutGet(GLUT_ELAPSED_TIME) / (float)milli / 2);

    // Keypresses
    if (global.keyPressTime + 20 < glutGet(GLUT_ELAPSED_TIME))
    {
        std::list<unsigned char> *pressed = keyboard->getPressed();
        for (std::list<unsigned char>::iterator i = pressed->begin();
             i != pressed->end(); ++i)
        {
            keyPress(*i);
        }
        global.keyPressTime = glutGet(GLUT_ELAPSED_TIME);
    }

    // Update Defence and Projectile Locations
    updateDefences(dt);
    updateProjectiles(dt);

    // Boat 1 Defences
    std::list<Defence *> *defences1 = boat1->getDefences();
    for (std::list<Defence *>::iterator d = defences1->begin();
         d != defences1->end(); ++d)
    {
        (*d)->increaseRadius();
    }
    // Boat 2 Defences
    std::list<Defence *> *defences2 = boat2->getDefences();
    for (std::list<Defence *>::iterator d = defences2->begin();
         d != defences2->end(); ++d)
    {
        (*d)->increaseRadius();
    }
    lastT = t;

    // Counter
    dt = t - global.lastFPSTime;
    if (dt > global.FPSInterval)
    {
        global.FPS = global.frames / dt;
        global.lastFPSTime = t;
        global.frames = 0;
    }

    glutPostRedisplay();
}

void keyDown(unsigned char key, int x, int y)
{
    // Keyboard single presses, else require holding
    switch (key)
    {
    case 27: // ESC - Quit Game
        exit(EXIT_SUCCESS);
        break;
    case 'n': // Wave - Normal
        wave->toggleNormal();
        break;
    case 't': // Wave - Tangent
        wave->toggleTangent();
        break;
    case 45: // Wave Segments (-)
        wave->decreaseNumSegments();
        break;
    case 61: // Wave Segments (+)
        wave->increaseNumSegments();
        break;
    case 'g': // Wave Toggle Animation
        wave->toggleAnimation();
        break;
    case 'q': // Wave Wireframe
        global.wireframe = !global.wireframe;
        break;
    default:
        keyboard->keyDown(key);
        break;
    }
}

void keyUp(unsigned char key, int x, int y)
{
    keyboard->keyUp(key);
}

void keyPress(unsigned char key)
{
    // Keypress functionality for simultaneous holding
    if (global.runnning)
    {
        switch (key)
        {
        case 'f': // Island Cannon Left
            island->cannonLeft(2.5);
            break;
        case 'h': // Island Cannon Right
            island->cannonRight(2.5);
            break;
        case 32: // Island Cannon Fire
            island->shoot();
            break;
        case 'a': // Boat Left Move Left
            boat1->moveLeft(0.01);
            break;
        case 'd': // Boat Left Move Right
            boat1->moveRight(0.01);
            break;
        case 'w': // Boat Left Cannon left
            boat1->cannonLeft(5);
            break;
        case 's': // Boat Left Cannon Right
            boat1->cannonRight(5);
            break;
        case 'e': // Boat Left Fire
            boat1->shoot();
            break;
        case 'r': // Boat Left Defence
            boat1->defence();
            break;
        case 'j': // Boat Right Move Left
            boat2->moveLeft(0.01);
            break;
        case 'l': // Boat Right Move Right
            boat2->moveRight(0.01);
            break;
        case 'i': // Boat Right Cannon Left
            boat2->cannonLeft(5);
            break;
        case 'k': // Boat Right Cannon Right
            boat2->cannonRight(5);
            break;
        case 'o': // Boat Right Fire
            boat2->shoot();
            break;
        case 'p': // Boat Right Defence
            boat2->defence();
            break;
        default:
            break;
        }
        glutPostRedisplay();
    }
}

void displayFPS()
{
    colour col = {1.0, 1.0, 1.0, 1.0};

    // Display FPS Counter
    glPushMatrix();
    std::string text = "FPS: " + std::to_string((int)global.FPS);
    int len = text.length();
    float dist = ((float)(len * 9) / global.windowWidth * 2);
    glTranslatef(0.95 - dist, 0.92, 0.0);
    glColor3f(col.r, col.g, col.b);
    glRasterPos2f(0, 0);
    for (int i = 0; i < len; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
    glPopMatrix();

    // Display FPS counter update time in seconds
    glPushMatrix();
    text = "Frame Time: " + std::to_string((int)global.FPSInterval);
    len = text.length();
    dist = ((float)(len * 9) / global.windowWidth * 2);
    glTranslatef(0.95 - dist, 0.86, 0.0);
    glColor3f(col.r, col.g, col.b);
    glRasterPos2f(0, 0);
    for (int i = 0; i < len; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
    glPopMatrix();

    // Display Tessellation Count
    glPushMatrix();
    text = "Tessellation: " + std::to_string(wave->getNumSegments());
    len = text.length();
    dist = ((float)(len * 9) / global.windowWidth * 2);
    glTranslatef(0.95 - dist, 0.80, 0.0);
    glColor3f(col.r, col.g, col.b);
    glRasterPos2f(0, 0);
    for (int i = 0; i < len; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
    glPopMatrix();
}

void updateProjectiles(float dt)
{
    // Update All Projectiles
    island->updateProjectile(dt);
    boat1->updateProjectile(dt);
    boat2->updateProjectile(dt);

    // Lambda function for collision detection
    auto projectileCollision = [&](std::list<Projectile *> *projectiles) {
        bool collision = false;

        for (std::list<Projectile *>::iterator p = projectiles->begin();
             p != projectiles->end(); ++p)
        {
            vec2f location = (*p)->getLocation();

            // Boat 1 Defences
            std::list<Defence *> *defences1 = boat1->getDefences();
            for (std::list<Defence *>::iterator d = defences1->begin();
                 d != defences1->end(); ++d)
            {
                if ((*p)->getCollision((*d)->getRadius(), (*d)->getLocation()))
                    collision = true;
            }
            // Boat 2 Defences
            std::list<Defence *> *defences2 = boat2->getDefences();
            for (std::list<Defence *>::iterator d = defences2->begin();
                 d != defences2->end(); ++d)
            {
                if ((*p)->getCollision((*d)->getRadius(), (*d)->getLocation()))
                    collision = true;
            }

            // Island
            if (!(*p)->getIsBoat())
            {
                // Boat 1 Collision
                if ((*p)->getCollision(boat1->getScale(), boat1->getLocation()))
                {
                    collision = true;
                    if (boat1->damage())
                        global.runnning = false;
                }
                // Boat 2 Collision
                if ((*p)->getCollision(boat2->getScale(), boat2->getLocation()))
                {
                    collision = true;
                    if (boat2->damage())
                        global.runnning = false;
                }
            }
            else
            {
                // Island Collision
                if (island->collision(location))
                {
                    collision = true;
                    if (island->damage())
                        global.runnning = false;
                }

                //Boats
                if ((*p)->getBoatNum() == 0)
                {
                    // Boat 2 Collision
                    if ((*p)->getCollision(boat2->getScale(),
                                           boat2->getLocation()))
                    {
                        collision = true;
                        if (boat2->damage())
                            global.runnning = false;
                    }
                }
                else if ((*p)->getBoatNum() == 1)
                {
                    // Boat 1 Collision
                    if ((*p)->getCollision(boat1->getScale(),
                                           boat1->getLocation()))
                    {
                        collision = true;
                        if (boat1->damage())
                            global.runnning = false;
                    }
                }
            }

            // Wave Collision
            if (wave->getYfromX(location.x) >= location.y)
                collision = true;

            if (collision)
            {
                projectiles->erase(p);
                return collision;
            }
        }
        return collision;
    };

    // Lambda function calls for all projectiles
    if (island->getProjectileExists())
        if (projectileCollision(island->getProjectiles()))
            island->removeProjectile();
    if (boat1->getProjectileExists())
        if (projectileCollision(boat1->getProjectiles()))
            boat1->removeProjectile();
    if (boat2->getProjectileExists())
        if (projectileCollision(boat2->getProjectiles()))
            boat2->removeProjectile();
}

void updateDefences(float dt)
{
    // Update All Defences
    boat1->updateDefence(dt);
    boat2->updateDefence(dt);

    // Lambda function for collision detection
    auto defenceCollision = [&](std::list<Defence *> *defences) {
        bool collision = false;

        for (std::list<Defence *>::iterator d = defences->begin();
             d != defences->end(); ++d)
        {
            vec2f location = (*d)->getLocation();
            // Wave Collision
            if (wave->getYfromX(location.x) >= location.y)
            {
                defences->erase(d);
                collision = true;
                break;
            }
        }

        return collision;
    };

    // Lambda function calls for all defences
    if (boat1->getDefenceExists())
        if (defenceCollision(boat1->getDefences()))
            boat1->removeDefence();
    if (boat2->getDefenceExists())
        if (defenceCollision(boat2->getDefences()))
            boat2->removeDefence();
}

// Draw funcitons

void drawAxis(float length)
{
    // X Axis
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(length, 0, 0);
    glEnd();

    // Y Axis
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, length, 0);
    glEnd();

    // Z Axis
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, length);
    glEnd();
}

void drawCircle(vec2f location, float r)
{
    const float numSegments = 50;

    float x = location.x;
    float y = location.y;

    // Drawing segments of circle
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numSegments; i++)
    {
        float theta = 2.0 * M_PI * float(i) / float(numSegments);
        float x1 = cos(theta) * r;
        float y1 = sin(theta) * r;
        glVertex2f(x1 + x, y1 + y);
    }
    glEnd();
}

// Calc functions

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