#include "triangle.h"

#include <algorithm>
#include <utility>
#include <vector>

using namespace std;


void Triangle::drawTriangle(pair<int, int> p0, pair<int, int> p1, pair<int, int> p2, Color color, QPainter& painter)
{
    if (p0.second == p1.second && p0.second == p2.second) return; // bad triangles )

    // sort points
    if (p0.second > p1.second) std::swap(p0, p1);
    if (p0.second > p2.second) std::swap(p0, p2);
    if (p1.second > p2.second) std::swap(p1, p2);

    int totalHeight = p2.second - p0.second;

    for (int i = 0; i < totalHeight; ++i)
    {
        bool secondHalf = i > p1.second - p0.second || p1.second == p0.second;
        int segmentHeight = secondHalf ? p2.second - p1.second : p1.second - p0.second;
        float alpha = (float)i / totalHeight,
            beta = (float)(i - (secondHalf ? p1.second - p0.second : 0)) / segmentHeight;

        std::pair<int, int> A = std::make_pair(p0.first + (p2.first - p0.first) * alpha, p0.second + (p2.second - p0.second) * alpha);
        std::pair<int, int> B = (secondHalf) ? std::make_pair(p1.first + (p2.first - p1.first) * beta, p1.second + (p2.second - p1.second) * beta) :
                                    std::make_pair(p0.first + (p1.first - p0.first) * beta, p0.second + (p1.second - p0.second) * beta);

        if (A.first > B.first) std::swap(A, B);

        for (int j = A.first; j <= B.first; ++j)
        {
            painter.setPen(QColor(color.r, color.g, color.b));
            painter.drawPoint(j, p0.second + i);
        }
    }
}
