/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *imagedraw;
    QLabel *io_nmc0;
    QLabel *io_nmc1;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1010, 621);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        imagedraw = new QLabel(centralwidget);
        imagedraw->setObjectName(QString::fromUtf8("imagedraw"));
        imagedraw->setGeometry(QRect(0, 0, 600, 600));
        io_nmc0 = new QLabel(centralwidget);
        io_nmc0->setObjectName(QString::fromUtf8("io_nmc0"));
        io_nmc0->setGeometry(QRect(620, 60, 351, 201));
        io_nmc0->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        io_nmc1 = new QLabel(centralwidget);
        io_nmc1->setObjectName(QString::fromUtf8("io_nmc1"));
        io_nmc1->setGeometry(QRect(620, 350, 341, 191));
        io_nmc1->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1010, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        imagedraw->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        io_nmc0->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        io_nmc1->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
