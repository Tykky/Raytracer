#include "Mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::Mainwindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

