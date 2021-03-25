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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "customlable.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *lable_fps;
    QLabel *lable_hw;
    QLabel *lable_fw;
    QLabel *lable_host;
    QLabel *lable_ip;
    QPushButton *find_corners;
    QPushButton *show_roi;
    QPushButton *switch_mode;
    QPushButton *fps_chart;
    QLineEdit *line_gain;
    QPushButton *set_gain;
    QLabel *label;
    QLineEdit *line_threshold;
    QLabel *label_2;
    QPushButton *set_threshold;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *topleft_x;
    QLineEdit *topleft_y;
    QLineEdit *bottomright_x;
    QLineEdit *bottomright_y;
    QPushButton *set_roi;
    QPushButton *confirm_roi;
    QPushButton *reset_roi;
    QListView *listView;
    Customlable *label_img;
    QListView *listView_2;
    QListView *listView_3;
    QListView *listView_4;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *logo;
    QMenuBar *menubar;
    QMenu *menuCamera_Calibratie;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1190, 620);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        lable_fps = new QLabel(centralwidget);
        lable_fps->setObjectName(QString::fromUtf8("lable_fps"));
        lable_fps->setGeometry(QRect(685, 50, 230, 20));
        lable_hw = new QLabel(centralwidget);
        lable_hw->setObjectName(QString::fromUtf8("lable_hw"));
        lable_hw->setGeometry(QRect(685, 160, 230, 20));
        lable_fw = new QLabel(centralwidget);
        lable_fw->setObjectName(QString::fromUtf8("lable_fw"));
        lable_fw->setGeometry(QRect(685, 135, 230, 20));
        lable_host = new QLabel(centralwidget);
        lable_host->setObjectName(QString::fromUtf8("lable_host"));
        lable_host->setGeometry(QRect(685, 80, 230, 20));
        lable_ip = new QLabel(centralwidget);
        lable_ip->setObjectName(QString::fromUtf8("lable_ip"));
        lable_ip->setGeometry(QRect(685, 105, 230, 20));
        find_corners = new QPushButton(centralwidget);
        find_corners->setObjectName(QString::fromUtf8("find_corners"));
        find_corners->setGeometry(QRect(685, 230, 230, 30));
        show_roi = new QPushButton(centralwidget);
        show_roi->setObjectName(QString::fromUtf8("show_roi"));
        show_roi->setGeometry(QRect(935, 335, 230, 30));
        switch_mode = new QPushButton(centralwidget);
        switch_mode->setObjectName(QString::fromUtf8("switch_mode"));
        switch_mode->setGeometry(QRect(685, 300, 230, 30));
        fps_chart = new QPushButton(centralwidget);
        fps_chart->setObjectName(QString::fromUtf8("fps_chart"));
        fps_chart->setGeometry(QRect(685, 265, 230, 30));
        line_gain = new QLineEdit(centralwidget);
        line_gain->setObjectName(QString::fromUtf8("line_gain"));
        line_gain->setGeometry(QRect(685, 365, 80, 25));
        set_gain = new QPushButton(centralwidget);
        set_gain->setObjectName(QString::fromUtf8("set_gain"));
        set_gain->setGeometry(QRect(775, 365, 80, 25));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(685, 340, 230, 20));
        line_threshold = new QLineEdit(centralwidget);
        line_threshold->setObjectName(QString::fromUtf8("line_threshold"));
        line_threshold->setGeometry(QRect(685, 425, 80, 25));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(685, 400, 230, 20));
        set_threshold = new QPushButton(centralwidget);
        set_threshold->setObjectName(QString::fromUtf8("set_threshold"));
        set_threshold->setGeometry(QRect(775, 425, 80, 25));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(935, 50, 230, 20));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(935, 75, 20, 25));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(935, 105, 20, 25));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(935, 140, 230, 20));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(935, 165, 20, 25));
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(935, 195, 20, 25));
        topleft_x = new QLineEdit(centralwidget);
        topleft_x->setObjectName(QString::fromUtf8("topleft_x"));
        topleft_x->setGeometry(QRect(955, 75, 91, 25));
        topleft_y = new QLineEdit(centralwidget);
        topleft_y->setObjectName(QString::fromUtf8("topleft_y"));
        topleft_y->setGeometry(QRect(955, 105, 91, 25));
        bottomright_x = new QLineEdit(centralwidget);
        bottomright_x->setObjectName(QString::fromUtf8("bottomright_x"));
        bottomright_x->setGeometry(QRect(955, 165, 91, 25));
        bottomright_y = new QLineEdit(centralwidget);
        bottomright_y->setObjectName(QString::fromUtf8("bottomright_y"));
        bottomright_y->setGeometry(QRect(955, 195, 91, 25));
        set_roi = new QPushButton(centralwidget);
        set_roi->setObjectName(QString::fromUtf8("set_roi"));
        set_roi->setGeometry(QRect(935, 230, 230, 30));
        confirm_roi = new QPushButton(centralwidget);
        confirm_roi->setObjectName(QString::fromUtf8("confirm_roi"));
        confirm_roi->setGeometry(QRect(935, 265, 230, 30));
        reset_roi = new QPushButton(centralwidget);
        reset_roi->setObjectName(QString::fromUtf8("reset_roi"));
        reset_roi->setGeometry(QRect(935, 300, 230, 30));
        listView = new QListView(centralwidget);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(930, 45, 240, 325));
        label_img = new Customlable(centralwidget);
        label_img->setObjectName(QString::fromUtf8("label_img"));
        label_img->setEnabled(true);
        label_img->setGeometry(QRect(25, 50, 640, 512));
        listView_2 = new QListView(centralwidget);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setGeometry(QRect(20, 45, 650, 522));
        listView_3 = new QListView(centralwidget);
        listView_3->setObjectName(QString::fromUtf8("listView_3"));
        listView_3->setGeometry(QRect(680, 45, 240, 140));
        listView_4 = new QListView(centralwidget);
        listView_4->setObjectName(QString::fromUtf8("listView_4"));
        listView_4->setGeometry(QRect(680, 225, 240, 230));
        label_9 = new QLabel(centralwidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(680, 200, 240, 20));
        label_9->setTextFormat(Qt::PlainText);
        label_9->setScaledContents(false);
        label_10 = new QLabel(centralwidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(20, 20, 650, 20));
        label_11 = new QLabel(centralwidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(680, 20, 240, 20));
        label_12 = new QLabel(centralwidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(930, 20, 240, 20));
        logo = new QLabel(centralwidget);
        logo->setObjectName(QString::fromUtf8("logo"));
        logo->setGeometry(QRect(930, 477, 240, 90));
        logo->setPixmap(QPixmap(QString::fromUtf8("../img/marelec_30.png")));
        MainWindow->setCentralWidget(centralwidget);
        listView->raise();
        reset_roi->raise();
        set_roi->raise();
        listView_4->raise();
        listView_3->raise();
        lable_fps->raise();
        lable_hw->raise();
        lable_fw->raise();
        lable_host->raise();
        lable_ip->raise();
        find_corners->raise();
        show_roi->raise();
        switch_mode->raise();
        fps_chart->raise();
        line_gain->raise();
        set_gain->raise();
        label->raise();
        line_threshold->raise();
        label_2->raise();
        set_threshold->raise();
        label_8->raise();
        label_4->raise();
        topleft_y->raise();
        confirm_roi->raise();
        label_6->raise();
        bottomright_x->raise();
        topleft_x->raise();
        bottomright_y->raise();
        label_3->raise();
        label_5->raise();
        label_7->raise();
        listView_2->raise();
        label_img->raise();
        label_9->raise();
        label_10->raise();
        label_11->raise();
        label_12->raise();
        logo->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1190, 21));
        menuCamera_Calibratie = new QMenu(menubar);
        menuCamera_Calibratie->setObjectName(QString::fromUtf8("menuCamera_Calibratie"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuCamera_Calibratie->menuAction());
        menuCamera_Calibratie->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lable_fps->setText(QCoreApplication::translate("MainWindow", "Current FPS: ", nullptr));
        lable_hw->setText(QCoreApplication::translate("MainWindow", "Hardware version: ", nullptr));
        lable_fw->setText(QCoreApplication::translate("MainWindow", "Firmware version: ", nullptr));
        lable_host->setText(QCoreApplication::translate("MainWindow", "Host: ", nullptr));
        lable_ip->setText(QCoreApplication::translate("MainWindow", "IP: ", nullptr));
        find_corners->setText(QCoreApplication::translate("MainWindow", "Find Corners", nullptr));
        show_roi->setText(QCoreApplication::translate("MainWindow", "Show Roi", nullptr));
        switch_mode->setText(QCoreApplication::translate("MainWindow", "Switch Mode", nullptr));
        fps_chart->setText(QCoreApplication::translate("MainWindow", "FPS Chart", nullptr));
        set_gain->setText(QCoreApplication::translate("MainWindow", "Apply", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Set Gain", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Set Threshold", nullptr));
        set_threshold->setText(QCoreApplication::translate("MainWindow", "Apply", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Top Left", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "x: ", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "y: ", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Bottom Right", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "x: ", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "y: ", nullptr));
        set_roi->setText(QCoreApplication::translate("MainWindow", "Set ROI", nullptr));
        confirm_roi->setText(QCoreApplication::translate("MainWindow", "Confirm", nullptr));
        reset_roi->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        label_img->setText(QString());
        label_9->setText(QCoreApplication::translate("MainWindow", "Features", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Visualisation", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Information", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "ROI", nullptr));
        logo->setText(QString());
        menuCamera_Calibratie->setTitle(QCoreApplication::translate("MainWindow", "Camera Calibratie", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
