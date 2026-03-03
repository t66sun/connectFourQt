#include "startwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    startwindow s;
    s.setWindowTitle("Start Menu");
    s.show();
    //MainWindow w;
    //w.setWindowTitle("Connect Four");
    //w.show();
    return a.exec();
}
