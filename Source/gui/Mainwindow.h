#ifndef RAYTRACER_MAINWINDOW_H
#define RAYTRACER_MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Mainwindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::Mainwindow *ui;
};


#endif //RAYTRACER_MAINWINDOW_H
