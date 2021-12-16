#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include "Databinding.h"
#include "Settings.h"
#include <QGLFormat>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    settings.loadSettingsOrDefaults();
    QGLFormat qglFormat;
    qglFormat.setVersion(4, 3);
    qglFormat.setProfile(QGLFormat::CoreProfile);
    qglFormat.setSampleBuffers(true);
    ui->setupUi(this);

    dataBind();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dataBind() {
#define BIND(b) { \
    DataBinding *_b = (b); \
    m_bindings.push_back(_b); \
    assert(connect(_b, SIGNAL(dataChanged()), this, SLOT(settingsChanged()))); \
}


#undef BIND
}

void MainWindow::settingsChanged() {
    // Left empty for now, change when needed for updating new sphere data
}

void MainWindow::on_addSphereButton_clicked() {
    std::cout << "Sphere button clicked" << std::endl;
}

void MainWindow::on_clearButton_clicked() {
    std::cout << "Clear button clicked" << std::endl;
}

void MainWindow::on_shakeButton_clicked() {
    std::cout << "Shake button clicked" << std::endl;
}

void MainWindow::on_settleButton_clicked() {
    std::cout << "Settle button clicked" << std::endl;
}
