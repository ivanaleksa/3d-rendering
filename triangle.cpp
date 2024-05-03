#include "triangle.h"

#include <algorithm>
#include <utility>
#include <vector>

using namespace std;


Color Triangle::interpolateColor(const Color& c1, const Color& c2, double t)
{
    Color result;
    result.r = c1.r * t + c2.r * (1 - t);
    result.g = c1.g * t + c2.g * (1 - t);
    result.b = c1.b * t + c2.b * (1 - t);
    return result;
}

void Triangle::sortYPoint(pair<int, int>& p1, pair<int, int>& p2, pair<int, int>& p3)
{
    vector<pair<int, int>> points = {p1, p2, p3};

    sort(points.begin(), points.end(), [](const pair<int, int>& a, const pair<int, int>& b) { return a.second < b.second; });

    p1 = points[0];
    p2 = points[1];
    p3 = points[2];
}

void Triangle::drawTriangle(pair<int, int> p0, pair<int, int> p1, pair<int, int> p2, Color color1, Color color2, QPainter& painter)
{
    sortYPoint(p0, p1, p2);

    int delta_t = p2.second - p0.second,
        delta_s = p1.second - p0.second;

    for (int y = p0.second; y < p1.second; ++y)
    {
        double alpha = (double)(y - p0.second) / delta_t,
            beta = (double)(y - p0.second) / delta_s;

        int startX = p0.first + (p2.first - p0.first) * alpha,
            endX = p0.first + (p1.first - p0.first) * beta;

        if (endX < startX)
            std::swap(endX, startX);

        for (int x = startX; x <= endX; ++x)
        {
            double t = (double)(endX - x) / (endX - startX);
            Color inter_color = interpolateColor(color1, color2, t);

            painter.setPen(QColor(inter_color.r, inter_color.g, inter_color.b));
            painter.drawPoint(x, y);
        }
    }

    delta_s = p2.second - p1.second;

    if (delta_s != 0)
    {
        for (int y = p2.second; y >= p1.second; --y)
        {
            double alpha = (double)(y - p2.second) / delta_t,
                beta = (double)(y - p2.second) / delta_s;

            int startX = p2.first + (p2.first - p0.first) * alpha,
                endX = p2.first + (p2.first - p1.first) * beta;

            if (endX < startX)
                std::swap(endX, startX);

            for (int x = startX; x <= endX; ++x)
            {
                double t = (double)(endX - x) / (endX - startX);
                Color inter_color = interpolateColor(color1, color2, t);

                painter.setPen(QColor(inter_color.r, inter_color.g, inter_color.b));
                painter.drawPoint(x, y);
            }
        }
    }
}
