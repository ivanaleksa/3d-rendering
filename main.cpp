#include "mainwindow.h"
#include "filereader.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    FileReader data("1.obj");
    w.show();
    return a.exec();
}
