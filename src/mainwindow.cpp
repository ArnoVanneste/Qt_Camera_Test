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
    ui->show_roi->setCheckable(true);

    server = new Udp();
    server->chooseCam();

    server->moveToThread(&read);
    connect(server, &Udp::hasToRender, this, &MainWindow::hasToRender);
    read.start();

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [&]() {

        ui->lable_fps->setText("Current FPS: " + QString::number(server->getFps()));
        ui->lable_host->setText("Host: " + server->getPosition(server->getAddr()));
        ui->lable_ip->setText("IP: " + server->getAddr());
        ui->lable_fw->setText("Firmware version: " + QString::number(server->fwVersion / 256) + "." + QString::number(server->fwVersion % 256));
        ui->lable_hw->setText("Hardware version: " + QString::number(server->hwVersion % 256));

        this->fpsChart->setFps(server->getFps());

    });

    timer->start(1000);

    boardtest = new Checkerboardtest();
    boardtest->moveToThread(&checkerboard);

    chtimer = new QTimer(this);

    connect(chtimer, &QTimer::timeout, this, [&]() {

        this->renderPoints(boardtest->calc(getImage()));

    });

    checkerboard.start();

    this->curCamMode = ECameraModes::PICTURE_MODE;

    this->fpsChart = new FpsChart();

    QPixmap logo("D:\\repos\\Qt_Camera_Test\\src\\img\\marelec_30.png");
    ui->logo->setPixmap(logo);
    ui->logo->setStyleSheet("background-color: rgba(0,0,0,0%)");

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

        const unsigned char * buff = this->curCamMode == ECameraModes::LASERMODE ?
//                    (const unsigned char *)server->getLaser() :
                    (const unsigned char *)server->getBuffer() :
                    (const unsigned char *)server->getBuffer();

        image = QImage(buff, 1280, 1024, QImage::Format_Grayscale8);
        //image.mirrored();
        image.setDotsPerMeterX(10000);
        image.setDotsPerMeterY(10000);
        QPixmap pix = QPixmap::fromImage(image);
        QPixmap scaled = pix.scaled(640,512, Qt::KeepAspectRatio);
        ui->label_img->setPixmap(scaled);
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

void MainWindow::setmode (ECameraModes mode) {
    this->curCamMode = mode;
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

void MainWindow::on_show_roi_clicked(bool checked)
{
    if (checked) {
        ui->label_img->showRoi = true;
    } else {
        ui->label_img->showRoi = false;
    }
}

//void MainWindow::on_increase_roi_width_clicked()
//{
//    ui->label_img->setWidth(ui->label_img->getWidth() + 5);
//    ui->label_img->center();
//    ui->roi_width->setText("ROI width: " + QString::number(ui->label_img->getWidth()));
//}

//void MainWindow::on_decrease_roi_width_clicked()
//{
//    ui->label_img->setWidth(ui->label_img->getWidth() - 5);
//    ui->label_img->center();
//    ui->roi_width->setText("ROI width: " + QString::number(ui->label_img->getWidth()));
//}

//void MainWindow::on_increase_roi_height_clicked()
//{
//    ui->label_img->setHeight(ui->label_img->getHeight() + 5);
//    ui->label_img->center();
//    ui->roi_height->setText("ROI height: " + QString::number(ui->label_img->getHeight()));
//}

//void MainWindow::on_decrease_roi_height_clicked()
//{
//    ui->label_img->setHeight(ui->label_img->getHeight() - 5);
//    ui->label_img->center();
//    ui->roi_height->setText("ROI height: " + QString::number(ui->label_img->getHeight()));
//}



void MainWindow::on_switch_mode_clicked()
{
    if(this->curCamMode == ECameraModes::PICTURE_MODE) {
        server->setRoi(0, 1280, 0, 1024,1,1,6,0, Udp::SET_THRESHOLD|Udp::SEND_LASER);
        setmode(ECameraModes::LASERMODE);

        // timeout
        Sleep(1000);
    } else {
        server->setRoi(0, 1280, 0, 1024,1,1,6,0, Udp::SEND_PICTURE|Udp::SEND_LASER);
        setmode(ECameraModes::PICTURE_MODE);

        // timeout
        Sleep(1000);
    }
}

void MainWindow::on_fps_chart_clicked()
{
    this->fpsChart->show();
}

void MainWindow::on_test_clicked()
{
    server->setRoi(0, 600, 0 , 400, 1,1,1,0, Udp::SET_THRESHOLD|Udp::SEND_LASER);
}

void MainWindow::on_set_gain_clicked()
{
    server->setGain((ui->line_gain->text()).toInt());
    ui->line_gain->setText("");
}

void MainWindow::on_set_threshold_clicked()
{
    server->setThreshold((ui->line_threshold->text()).toInt());
    ui->line_threshold->setText("");
}

void MainWindow::on_set_roi_clicked()
{

    ui->label_img->setRoiRect(QRect(ui->topleft_x->text().toInt() / 2, ui->topleft_y->text().toInt() / 2, ui->bottomright_x->text().toInt() / 2 - ui->topleft_x->text().toInt() / 2, ui->bottomright_y->text().toInt() / 2 - ui->topleft_y->text().toInt() / 2));

}

void MainWindow::on_reset_roi_clicked()
{
    server->setTopleftX(0);
    server->setTopleftY(0);
    server->setBottomrightX(1280);
    server->setBottomrightY(1024);

    server->setRoi(0, 1280, 0, 1024,1,1,6,0, Udp::SEND_PICTURE|Udp::SEND_LASER);
}

void MainWindow::on_confirm_roi_clicked()
{
    server->setTopleftX(ui->topleft_x->text().toInt());
    server->setTopleftY(ui->topleft_y->text().toInt());
    server->setBottomrightX(ui->bottomright_x->text().toInt());
    server->setBottomrightY(ui->bottomright_y->text().toInt());

    server->setRoi(ui->topleft_x->text().toInt(),
                   (ui->bottomright_x->text().toInt() - ui->topleft_x->text().toInt()),
                   ui->topleft_y->text().toInt(),
                   (ui->bottomright_y->text().toInt() - ui->topleft_y->text().toInt()),
                   1,1,6,0, Udp::SEND_PICTURE|Udp::SEND_LASER
                   );

    ui->label_img->update();
}
