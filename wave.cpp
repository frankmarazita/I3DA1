#include "wave.h"

Wave::Wave(float windowSize, float numSegments, float a, float b, float c, float d)
{
    this->windowSize = windowSize;
    this->numSegments = numSegments;
    this->segmentSize = windowSize * 2 / numSegments;

    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;

    calcVerticies();
}

void Wave::draw()
{
    // Draw Wave
    glColor4f(col.r, col.g, col.b, col.a);
    glBegin(GL_QUAD_STRIP);
    for (std::list<vec2f>::iterator i = verticies.begin();
         i != verticies.end(); ++i)
    {
        // For each of the already generated verticies, update their y value
        glVertex2f((*i).x, a * sin(b * (*i).x + c) + d);
        glVertex2f((*i).x, -1);
    }
    glEnd();

    if (normal || tangent)
    {
        for (std::list<vec2f>::iterator i = verticies.begin();
             i != verticies.end(); ++i)
        {
            // For each of the generated verticies: Draw Normal and/or Tangent
            float x = (*i).x;
            float y = a * sin(b * x + c) + d;
            float m = a * b * cos(b * x + c);
            if (normal)
                drawVector({x, y}, m, 0.1, true);
            if (tangent)
                drawVector({x, y}, m, 0.1, false);
        }
    }
}

void Wave::moveWave(float amount)
{
    // Set the wave displacement c value
    c = amount;
}

void Wave::calcVerticies()
{
    verticies.clear();
    // Calculate the initial verticies locations based on num segments
    for (int i = 0; i <= numSegments; i++)
    {
        float x = -windowSize + segmentSize * i;
        float y = a * sin(b * x + c) + d;
        verticies.push_back({x, y});
    }
}

float Wave::getYfromX(float x)
{
    return a * sin(b * x + c) + d;
}

float Wave::getGrad(float x)
{
    return radToDeg(a * b * cos(b * x + c) / (M_PI / 2));
}

bool Wave::getAnimate()
{
    return animate;
}

int Wave::getNumSegments()
{
    return numSegments;
}

void Wave::toggleNormal()
{
    normal = !normal;
}

void Wave::toggleTangent()
{
    tangent = !tangent;
}

void Wave::toggleAnimation()
{
    animate = !animate;
}

void Wave::increaseNumSegments()
{
    numSegments *= 2;
    segmentSize = windowSize * 2 / numSegments;
    calcVerticies();
}

void Wave::decreaseNumSegments()
{
    // Halve the number of segments
    int newNumSegments = numSegments / 2;
    // Check for minimum 4
    if (newNumSegments >= 4)
    {
        numSegments = newNumSegments;
        segmentSize = windowSize * 2 / numSegments;
        // Recalulate verticies
        calcVerticies();
    }
}

void Wave::drawVector(vec2f point, float m, float s, bool normalize)
{
    float x = point.x;
    float y = point.y;

    float x2 = 0;
    float y2 = 0;

    // Calculate Normals and Tangents
    if (normalize)
    {
        glColor3f(0, 1, 0);

        m = -(1 / m);
        if (m < 0)
        {
            x2 = x - cos(gradToRad(m)) * s;
            y2 = y - sin(gradToRad(m)) * s;
        }
        else
        {
            x2 = x + cos(gradToRad(m)) * s;
            y2 = y + sin(gradToRad(m)) * s;
        }
    }
    else
    {
        glColor3f(1, 0, 0);

        if (m < 0)
        {
            x2 = x - cos(gradToRad(m)) * s;
            y2 = y - sin(gradToRad(m)) * s;
        }
        else
        {
            x2 = x + cos(gradToRad(m)) * s;
            y2 = y + sin(gradToRad(m)) * s;
        }
    }

    // Draw Normals and Tangents
    glBegin(GL_LINE_STRIP);
    glVertex2f(x, y);
    glVertex2f(x2, y2);
    glEnd();
}