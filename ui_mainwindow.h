/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include "view.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    View *view;
    QGroupBox *attenuationGroupBox;
    QSlider *radiusSlider;
    QLabel *massLabel;
    QLabel *bouncinessLabel;
    QLineEdit *bouncinessEdit;
    QSlider *massSlider;
    QLineEdit *massEdit;
    QSlider *bouncinessSlider;
    QLineEdit *radiusEdit;
    QLabel *radiusLabel;
    QPushButton *addSphereButton;
    QPushButton *shakeButton;
    QPushButton *clearButton;
    QPushButton *settleButton;
    QLabel *sphereCountLabel;
    QLineEdit *sphereCountEdit;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1251, 771);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        view = new View(centralWidget);
        view->setObjectName(QString::fromUtf8("view"));
        view->setGeometry(QRect(290, 10, 951, 751));
        attenuationGroupBox = new QGroupBox(centralWidget);
        attenuationGroupBox->setObjectName(QString::fromUtf8("attenuationGroupBox"));
        attenuationGroupBox->setEnabled(true);
        attenuationGroupBox->setGeometry(QRect(10, 20, 261, 361));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(attenuationGroupBox->sizePolicy().hasHeightForWidth());
        attenuationGroupBox->setSizePolicy(sizePolicy);
        attenuationGroupBox->setMinimumSize(QSize(0, 105));
        attenuationGroupBox->setCheckable(false);
        radiusSlider = new QSlider(attenuationGroupBox);
        radiusSlider->setObjectName(QString::fromUtf8("radiusSlider"));
        radiusSlider->setGeometry(QRect(150, 20, 101, 22));
        radiusSlider->setOrientation(Qt::Horizontal);
        massLabel = new QLabel(attenuationGroupBox);
        massLabel->setObjectName(QString::fromUtf8("massLabel"));
        massLabel->setGeometry(QRect(10, 40, 61, 41));
        bouncinessLabel = new QLabel(attenuationGroupBox);
        bouncinessLabel->setObjectName(QString::fromUtf8("bouncinessLabel"));
        bouncinessLabel->setGeometry(QRect(10, 70, 91, 41));
        bouncinessEdit = new QLineEdit(attenuationGroupBox);
        bouncinessEdit->setObjectName(QString::fromUtf8("bouncinessEdit"));
        bouncinessEdit->setGeometry(QRect(100, 80, 40, 21));
        bouncinessEdit->setMinimumSize(QSize(40, 0));
        bouncinessEdit->setMaximumSize(QSize(40, 16777215));
        massSlider = new QSlider(attenuationGroupBox);
        massSlider->setObjectName(QString::fromUtf8("massSlider"));
        massSlider->setGeometry(QRect(150, 50, 101, 22));
        massSlider->setOrientation(Qt::Horizontal);
        massEdit = new QLineEdit(attenuationGroupBox);
        massEdit->setObjectName(QString::fromUtf8("massEdit"));
        massEdit->setGeometry(QRect(100, 50, 40, 21));
        massEdit->setMinimumSize(QSize(40, 0));
        massEdit->setMaximumSize(QSize(40, 16777215));
        bouncinessSlider = new QSlider(attenuationGroupBox);
        bouncinessSlider->setObjectName(QString::fromUtf8("bouncinessSlider"));
        bouncinessSlider->setGeometry(QRect(150, 80, 101, 22));
        bouncinessSlider->setOrientation(Qt::Horizontal);
        radiusEdit = new QLineEdit(attenuationGroupBox);
        radiusEdit->setObjectName(QString::fromUtf8("radiusEdit"));
        radiusEdit->setGeometry(QRect(100, 20, 40, 21));
        radiusEdit->setMinimumSize(QSize(40, 0));
        radiusEdit->setMaximumSize(QSize(40, 16777215));
        radiusLabel = new QLabel(attenuationGroupBox);
        radiusLabel->setObjectName(QString::fromUtf8("radiusLabel"));
        radiusLabel->setGeometry(QRect(10, 10, 91, 41));
        addSphereButton = new QPushButton(attenuationGroupBox);
        addSphereButton->setObjectName(QString::fromUtf8("addSphereButton"));
        addSphereButton->setGeometry(QRect(10, 130, 111, 32));
        shakeButton = new QPushButton(attenuationGroupBox);
        shakeButton->setObjectName(QString::fromUtf8("shakeButton"));
        shakeButton->setGeometry(QRect(10, 190, 111, 31));
        clearButton = new QPushButton(attenuationGroupBox);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setGeometry(QRect(10, 160, 111, 32));
        settleButton = new QPushButton(attenuationGroupBox);
        settleButton->setObjectName(QString::fromUtf8("settleButton"));
        settleButton->setGeometry(QRect(10, 220, 111, 31));
        sphereCountLabel = new QLabel(attenuationGroupBox);
        sphereCountLabel->setObjectName(QString::fromUtf8("sphereCountLabel"));
        sphereCountLabel->setGeometry(QRect(140, 130, 51, 31));
        sphereCountEdit = new QLineEdit(attenuationGroupBox);
        sphereCountEdit->setObjectName(QString::fromUtf8("sphereCountEdit"));
        sphereCountEdit->setGeometry(QRect(190, 130, 41, 31));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "A CS123 Final", nullptr));
        attenuationGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Sphere", nullptr));
        massLabel->setText(QCoreApplication::translate("MainWindow", "Mass", nullptr));
        bouncinessLabel->setText(QCoreApplication::translate("MainWindow", "Bounciness", nullptr));
        radiusLabel->setText(QCoreApplication::translate("MainWindow", "Radius", nullptr));
        addSphereButton->setText(QCoreApplication::translate("MainWindow", "Add Sphere", nullptr));
        shakeButton->setText(QCoreApplication::translate("MainWindow", "Shake", nullptr));
        clearButton->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        settleButton->setText(QCoreApplication::translate("MainWindow", "Settle", nullptr));
        sphereCountLabel->setText(QCoreApplication::translate("MainWindow", "Spheres", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
