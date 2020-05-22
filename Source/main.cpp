#include <QApplication>
#include "gui/Mainwindow.h"
#include <QPushButton>
#include <QFile>
#include <QTextStream>

using namespace std;

int main(int argc, char **argv) {
    QApplication app(argc, argv );

    // Load qdarkstyle
    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        app.setStyleSheet(ts.readAll());
    }

    MainWindow mainwindow;
    mainwindow.show();
    return app.exec();
}