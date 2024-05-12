#include "triangle.h"

#include <algorithm>
#include <utility>
#include <vector>

using namespace std;


void Triangle::drawTriangle(Vec3d<int> p0, Vec3d<int> p1, Vec3d<int> p2, Color color, QPainter& painter, int* zbuffer, int width)
{
    if (p0.getY() == p1.getY() && p0.getY() == p2.getY()) return; // bad triangles )

    // sort points
    if (p0.getY() > p1.getY()) std::swap(p0, p1);
    if (p0.getY() > p2.getY()) std::swap(p0, p2);
    if (p1.getY() > p2.getY()) std::swap(p1, p2);

    int totalHeight = p2.getY() - p0.getY();

    for (int i = 0; i < totalHeight; ++i)
    {
        bool secondHalf = i > p1.getY() - p0.getY() || p1.getY() == p0.getY();
        int segmentHeight = secondHalf ? p2.getY() - p1.getY() : p1.getY() - p0.getY();
        float alpha = (float)i / totalHeight,
            beta = (float)(i - (secondHalf ? p1.getY() - p0.getY() : 0)) / segmentHeight;

        Vec3d<int> A = p0 + Vec3d<float>(p2 - p0) * alpha;
        Vec3d<int> B = secondHalf ? p1 + Vec3d<float>(p2 - p1) * beta : p0 + Vec3d<float>(p1 - p0) * beta;

        if (A.getX() > B.getX()) std::swap(A, B);

        for (int j = A.getX(); j <= B.getX(); ++j)
        {
            float phi = (B.getX() == A.getX()) ? 1. : (float)(j - A.getX()) / (float)(B.getX() - A.getX());
            Vec3d<int> P = Vec3d<float>(A) + Vec3d<float>(B - A) * phi;

            P.setX(j);
            P.setY(p0.getY() + i);

            int idx = P.getX() + P.getY() * width;
            if (zbuffer[idx] < P.getZ())
            {
                zbuffer[idx] = P.getZ();
                painter.setPen(QColor(color.r, color.g, color.b));
                painter.drawPoint(P.getX(), P.getY());
            }
        }
    }
}
