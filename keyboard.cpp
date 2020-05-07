#include "keyboard.h"

Keyboard::Keyboard()
{
}

void Keyboard::keyDown(unsigned char key)
{
    pressed.push_back(key);
}

void Keyboard::keyUp(unsigned char key)
{
    // When a key is released, remove from list
    for (std::list<unsigned char>::iterator i = pressed.begin();
         i != pressed.end(); ++i)
        if (*i == key)
        {
            pressed.erase(i);
            break;
        }
}

std::list<unsigned char> *Keyboard::getPressed()
{
    return &pressed;
}