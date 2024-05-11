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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FileReader data("head.obj");

    QScreen *screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->size();

    QPixmap pixmap(screenSize);
    pixmap.fill(Qt::black);

    QPainter painter(&pixmap);
    painter.setPen(Qt::black);
    Triangle tr;

    std::vector<std::vector<int>> polygons = data.getPolygons();
    std::vector<Vec3d<float>> vertices = data.getVertices();

    Vec3d<float> lightDir(0,0,-1);
    for (int i = 0; i < (int)polygons.size(); ++i)
    {
        std::vector<int> triangleIndx = polygons[i];
        std::pair<int, int> screenCoord[3];
        Vec3d<float> worldCoords[3];
        for (int j = 0; j < 3; ++j)
        {
            Vec3d<float> v = vertices[triangleIndx[j]];
            screenCoord[j] = std::make_pair((v.getX() + 1) * screenSize.width() / 4,
                                                 (v.getY() + 1) * screenSize.width() / 4);
            worldCoords[j] = v;
        }
        Vec3d<float> n = (worldCoords[2] - worldCoords[0]) ^ (worldCoords[1] - worldCoords[0]);
        n.normalize();

        if (n * lightDir >= 0) {
            float intensity = n * lightDir;

            Color color = {intensity * 255, intensity * 255, intensity * 255};

            tr.drawTriangle(screenCoord[0], screenCoord[1], screenCoord[2], color, color, painter);
        }
    }

    pixmap = pixmap.transformed(QTransform().scale(1, -1));

    QLabel label;
    label.setPixmap(pixmap);
    label.showFullScreen();

    return a.exec();
}
