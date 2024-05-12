#include "triangle.h"

#include <algorithm>
#include <utility>
#include <QDebug>

using namespace std;


void Triangle::drawTriangle(Vec3d<int> p0, Vec3d<int> p1, Vec3d<int> p2, float ity0, float ity1, float ity2, QPainter& painter, int* zbuffer, int width, int height)
{
    if (p0.getY() == p1.getY() && p0.getY() == p2.getY()) return; // bad triangles )

    // sort points
    if (p0.getY() > p1.getY())
    {
        std::swap(p0, p1);
        std::swap(ity0, ity1);
    }
    if (p0.getY() > p2.getY())
    {
        std::swap(p0, p2);
        std::swap(ity0, ity2);
    }
    if (p1.getY() > p2.getY())
    {
        std::swap(p1, p2);
        std::swap(ity1, ity2);
    }

    int totalHeight = p2.getY() - p0.getY();

    for (int i = 0; i < totalHeight; ++i)
    {
        bool secondHalf = i > p1.getY() - p0.getY() || p1.getY() == p0.getY();
        int segmentHeight = secondHalf ? p2.getY() - p1.getY() : p1.getY() - p0.getY();
        float alpha = (float)i / totalHeight,
            beta = (float)(i - (secondHalf ? p1.getY() - p0.getY() : 0)) / segmentHeight;

        Vec3d<int> A = p0 + Vec3d<float>(p2 - p0) * alpha;
        Vec3d<int> B = secondHalf ? p1 + Vec3d<float>(p2 - p1) * beta : p0 + Vec3d<float>(p1 - p0) * beta;

        float ityA = ity0 + (ity2 - ity0) * alpha;
        float ityB = secondHalf ? ity1 + (ity2 - ity1) * beta : ity0 + (ity1 - ity0) * beta;

        if (A.getX() > B.getX())
        {
            std::swap(A, B);
            std::swap(ityA, ityB);
        }

        for (int j = A.getX(); j <= B.getX(); ++j)
        {
            float phi = (B.getX() == A.getX()) ? 1. : (float)(j - A.getX()) / (float)(B.getX() - A.getX());
            Vec3d<int> P = Vec3d<float>(A) + Vec3d<float>(B - A) * phi;
            float ityP = ityA + (ityB - ityA) * phi;

            P.setX(j);
            P.setY(p0.getY() + i);

            int idx = P.getX() + P.getY() * width;

            if (P.getX() >= width || P.getY() >= height || P.getX() < 0 || P.getY() < 0) continue;

            if (idx >= 0 && idx < width * height) {
                if (zbuffer[idx] < P.getZ()) {
                    zbuffer[idx] = P.getZ();
                    painter.setPen(QColor(0 * ityP, 172 * ityP, 17 * ityP));
                    painter.drawPoint(P.getX(), P.getY());
                }
            }
        }
    }
}
