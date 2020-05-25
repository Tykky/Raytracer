#include <QApplication>
#include "gui/Mainwindow.h"
#include <QPushButton>
#include <QFile>
#include <QTextStream>

using namespace std;

int main(int argc, char **argv) {
    QApplication app(argc, argv );
    MainWindow mainwindow;
    mainwindow.show();
    return app.exec();
}