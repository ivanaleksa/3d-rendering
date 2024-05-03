#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QPainter>
#include <QColor>

using namespace std;

struct Color {
    int r, g, b;
};


class Triangle
{
public:
    void drawTriangle(pair<int, int>, pair<int, int>, pair<int, int>, Color, Color, QPainter&);
    void sortYPoint(pair<int, int>&, pair<int, int>&, pair<int, int>&);
    Color interpolateColor(const Color&, const Color&, double);
};

#endif // TRIANGLE_H
