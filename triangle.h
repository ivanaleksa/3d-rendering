#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QPainter>
#include <QColor>

#include "geometry.h"

using namespace std;

struct Color {
    int r, g, b;
};


class Triangle
{
public:
    void drawTriangle(Vec3d<int>, Vec3d<int>, Vec3d<int>, float, float, float, QPainter&, int*, int, int);
};

#endif // TRIANGLE_H
