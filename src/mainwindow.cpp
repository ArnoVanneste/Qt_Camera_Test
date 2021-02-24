#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "udp.h"
#include "checkerboardtest.h"
#include "customlable.h"
#include <QPixmap>
#include <QThread>
#include <iostream>

#include <chrono>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->find_corners->setCheckable(true);

    server = new Udp();
    server->chooseCam();

    server->moveToThread(&read);
    connect(server, &Udp::hasToRender, this, &MainWindow::hasToRender);
    read.start();

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [&]() {

        ui->lable_fps->setText("Current FPS: " + server->getFps());
        ui->lable_host->setText("Host: " + server->getPosition(server->getAddr()));
        ui->lable_ip->setText("IP: " + server->getAddr());
        ui->lable_fw->setText("Firmware version: " + QString::number(server->fwVersion / 256) + "." + QString::number(server->fwVersion % 256));
        ui->lable_hw->setText("Hardware version: " + QString::number(server->hwVersion % 256));

    });

    timer->start(1000);

    boardtest = new Checkerboardtest();
    boardtest->moveToThread(&checkerboard);

    chtimer = new QTimer(this);

    connect(chtimer, &QTimer::timeout, this, [&]() {

        this->renderPoints(boardtest->calc(getImage()));

    });

    checkerboard.start();

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

        const unsigned char * buff = (const unsigned char *)server->getBuffer();
        image = QImage(buff, 1280, 1024, QImage::Format_Grayscale8);
        image.mirrored();
        image.setDotsPerMeterX(10000);
        image.setDotsPerMeterY(10000);
        QPixmap pix = QPixmap::fromImage(image);
        ui->label_img->setPixmap(pix);
        server->mutex.unlock();

    }
}

void MainWindow::renderPoints(const std::vector<cv::Point2f>& points) const {

    ui->label_img->setPoints(points);
    ui->label_img->update();

}

const QImage & MainWindow::getImage(){

    return image;

}


void MainWindow::on_find_corners_clicked(bool checked)
{
    if (checked) {
        chtimer->start(500);
        ui->label_img->drawPoints = true;
    } else {
        chtimer->stop();
        ui->label_img->drawPoints = false;
    }
}
