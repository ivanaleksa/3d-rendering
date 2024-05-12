#include "mainwindow.h"

#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QSize>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QDebug>

#include "filereader.h"
#include "triangle.h"
#include "geometry.h"


const int depth = 255;
int *zbuffer = nullptr;
Vec3d<float> lightDir(0,0,-1), camera(0, 0, 3);


Vec3d<float> m2v(Matrix m)
{
    return Vec3d<float>(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

Matrix v2m(Vec3d<float> v)
{
    Matrix m(4, 1);
    m[0][0] = v.getX();
    m[1][0] = v.getY();
    m[2][0] = v.getZ();
    m[3][0] = 1.f;
    return m;
}

Matrix viewport(int x, int y, int w, int h)
{
    Matrix m = Matrix::identity(4);
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = depth / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = depth / 2.f;
    return m;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FileReader data("head.obj");

    // initializinf of a screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->size();

    QPixmap pixmap(screenSize);
    pixmap.fill(Qt::black);

    QPainter painter(&pixmap);
    painter.setPen(Qt::black);
    Triangle tr;


    // initializing of zbuffer
    zbuffer = new int[screenSize.width() * screenSize.height()];
    for (int i = 0; i < screenSize.width() * screenSize.height(); ++i)
        zbuffer[i] = std::numeric_limits<int>::min();


    // drawing
    Matrix Projection = Matrix::identity(4);
    Matrix ViewPort = viewport(screenSize.width() / 8, screenSize.height() / 8, screenSize.width() * 3 / 4, screenSize.height() * 3 / 4);
    Projection[3][2] = -1.f / camera.getZ();

    std::vector<std::vector<int>> polygons = data.getPolygons();
    std::vector<Vec3d<float>> vertices = data.getVertices();

    for (int i = 0; i < (int)polygons.size(); ++i)
    {
        std::vector<int> triangleIndx = polygons[i];
        Vec3d<int> screenCoord[3];
        Vec3d<float> worldCoords[3];
        for (int j = 0; j < 3; ++j)
        {
            Vec3d<float> v = vertices[triangleIndx[j]];
            screenCoord[j] = m2v(ViewPort * Projection * v2m(v));
            worldCoords[j] = v;
        }
        Vec3d<float> n = (worldCoords[2] - worldCoords[0]) ^ (worldCoords[1] - worldCoords[0]);
        n.normalize();

        if (n * lightDir > 0) {
            float intensity = n * lightDir;

            Color color = {intensity * 255, intensity * 255, intensity * 255};

            tr.drawTriangle(screenCoord[0], screenCoord[1], screenCoord[2], color, painter, zbuffer, screenSize.width());
        }
    }

    pixmap = pixmap.transformed(QTransform().scale(1, -1));

    QLabel label;
    label.setPixmap(pixmap);
    label.showFullScreen();

    delete[] zbuffer;

    return a.exec();
}
