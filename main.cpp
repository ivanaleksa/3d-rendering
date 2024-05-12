#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QSize>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>

#include <vector>

#include "filereader.h"
#include "triangle.h"
#include "geometry.h"


const int depth = 255;
int* zbuffer = nullptr;
Vec3d<float> lightDir = Vec3d<float>(1, -1, 1).normalize(), camera(1, 1, 3), center(0, 0, 0);//center(3, 0.5, 0);


template<typename T>
Vec3d<T> cross(const Vec3d<T> &v1, const Vec3d<T> &v2)
{
    return Vec3d<T>{
        v1.getY() * v2.getZ() - v1.getZ() * v2.getY(),
        v1.getZ() * v2.getX() - v1.getX() * v2.getZ(),
        v1.getX() * v2.getY() - v1.getY() * v2.getX()
    };
}

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

Matrix lookat(Vec3d<float> eye, Vec3d<float> center, Vec3d<float> up) {
    Vec3d<float> z = (eye - center).normalize();
    Vec3d<float> x = cross(up, z).normalize();
    Vec3d<float> y = cross(z, x).normalize();
    Matrix Minv = Matrix::identity(4);
    Matrix Tr = Matrix::identity(4);
    for (int i=0; i<3; i++) {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];
        Tr[i][3] = -center[i];
    }
    return Minv * Tr;
}


class CustomWidget : public QWidget {
public:
    CustomWidget(const QPixmap& pixmap, QWidget *parent = nullptr) : QWidget(parent), m_pixmap(pixmap) {}

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.drawPixmap(0, 0, m_pixmap);
    }

    void keyPressEvent(QKeyEvent *event) override {
        switch (event->key()) {
        case Qt::Key_Left:
            // Handle left arrow key press
            // Example: move object to the left
            // DoSomething();
            break;
        case Qt::Key_Right:
            // Handle right arrow key press
            // Example: move object to the right
            // DoSomething();
            break;
        case Qt::Key_Up:
            // Handle up arrow key press
            // Example: move object up
            // DoSomething();
            break;
        case Qt::Key_Down:
            // Handle down arrow key press
            // Example: move object down
            // DoSomething();
            break;
        default:
            QWidget::keyPressEvent(event);
        }
    }

private:
    QPixmap m_pixmap;
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FileReader data("head.obj");

    // initializing of a screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->size();

    QPixmap pixmap(screenSize);
    pixmap.fill(Qt::gray);

    QPainter painter(&pixmap);
    painter.setPen(Qt::black);
    Triangle tr;

    // initializing of zbuffer
    int* zbuffer = new int[screenSize.width() * screenSize.height()];
    for (int i = 0; i < screenSize.width() * screenSize.height(); ++i)
        zbuffer[i] = std::numeric_limits<int>::min();

    // drawing
    Matrix ModelView = lookat(camera, center, Vec3d<float>(0, 1, 0));
    Matrix Projection = Matrix::identity(4);
    Matrix ViewPort = viewport(screenSize.width() / 8, screenSize.height() / 8, screenSize.width() * 3 / 4, screenSize.height() * 3 / 4);
    Projection[3][2] = -1.f / (camera - center).norm();

    std::vector<std::vector<int>> polygons = data.getPolygons(), polygonsNorms = data.getPolygonsNorms();
    std::vector<Vec3d<float>> vertices = data.getVertices(), norms = data.getNorms();

    for (int i = 0; i < (int)polygons.size(); ++i)
    {
        std::vector<int> triangleIndx = polygons[i], normsIndx = polygonsNorms[i];
        Vec3d<int> screenCoord[3];
        Vec3d<float> worldCoords[3];
        float intensity[3];
        for (int j = 0; j < 3; ++j)
        {
            Vec3d<float> v = vertices[triangleIndx[j]], norm = norms[normsIndx[j]];
            screenCoord[j] = m2v(ViewPort * Projection * ModelView * v2m(v));
            worldCoords[j] = v;
            intensity[j] = norm * lightDir;
        }
        tr.drawTriangle(screenCoord[0], screenCoord[1], screenCoord[2], intensity[0], intensity[1], intensity[2], painter, zbuffer, screenSize.width(), screenSize.height());
    }

    pixmap = pixmap.transformed(QTransform().scale(1, -1));

    CustomWidget widget(pixmap);
    widget.showFullScreen();

    delete[] zbuffer;

    return a.exec();
}
