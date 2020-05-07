#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <list>
#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Keyboard
{
public:
    Keyboard();

    void keyDown(unsigned char key);
    void keyUp(unsigned char key);

    std::list<unsigned char> *getPressed();

private:
    std::list<unsigned char> pressed;
};

#endif // KEYBOARD_H