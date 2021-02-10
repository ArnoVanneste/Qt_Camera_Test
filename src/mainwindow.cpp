#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "udp.h"
#include <QPixmap>
#include <QThread>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new Udp();
    server->chooseCam();

    server->moveToThread(&read);
    connect(server, &Udp::hasToRender, this, &MainWindow::hasToRender);
    read.start();
//    server->sendData("192.168.2.2");

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]() {
        ui->lable_fps->setText("Current FPS: " + server->getFps());
        ui->lable_host->setText("Host: " + server->getPosition(server->getAddr()));
        ui->lable_ip->setText("IP: " + server->getAddr());

        // sprintf(s,TEXT("%s v%i.%i"),szTitle, wParam/256,wParam%256);

        ui->lable_fw->setText("Firmware version: " + QString::number(server->fwVersion / 256) + "." + QString::number(server->fwVersion % 256));
        ui->lable_hw->setText("Hardware version: " + QString::number(server->hwVersion % 256));
    });
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    read.quit();
    read.wait();
    delete ui;
}

void MainWindow::hasToRender()
{
    if (server->mutex.tryLock()) {
//        qDebug() << "rendering";

        const unsigned char * buff = (const unsigned char *)server->getBuffer();
        QImage image (buff, 1280, 1024, QImage::Format_Grayscale8);
        image.mirrored();

        /* color table */
//            QVector<QRgb> colorTable (256);
//            for (unsigned short index = 0; index < colorTable.size(); index++) {
//                colorTable[index] = QColor(index, index, index, 255).rgb();
//            }

        // image.setColorTable(colorTable);

//            qDebug() << image.bitPlaneCount();
        image.setDotsPerMeterX(10000);
        image.setDotsPerMeterY(10000);
        QPixmap pix = QPixmap::fromImage(image);
        ui->label_img->setPixmap(pix);

        server->mutex.unlock();
//        qDebug() << "unlocked";
    }
}
