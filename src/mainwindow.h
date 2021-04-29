#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QMainWindow>
#include <QNetworkDatagram>
#include <QTimer>
#include <QPainter>

#include "customlable.h"
#include "udp.h"
#include "checkerboardtest.h"
#include "fpschart.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QThread read;
    QThread checkerboard;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    QImage getImage(void) const;

    enum ECameraModes {PICTURE_MODE, LASERMODE};
    void setmode (ECameraModes mode);

private:
    Ui::MainWindow *ui;
    Udp *server;
    Checkerboardtest *boardtest;
    QTimer *timer;
    QTimer *chtimer;
    QPixmap pix;
    QImage image;
    ECameraModes curCamMode;
    FpsChart *fpsChart;

    std::vector<QString> timeStamps;

    bool geenAntwoord;

    void renderPoints(const std::vector<cv::Point2f>& points) const;

private slots:
    void hasToRender(void);

    void on_find_corners_clicked(bool checked);
    void on_show_roi_clicked(bool checked);
    void on_switch_mode_clicked();
    void on_fps_chart_clicked();
    void on_test_clicked();
    void on_set_gain_clicked();
    void on_set_threshold_clicked();
    void on_set_roi_clicked();
    void on_reset_roi_clicked();
    void on_confirm_roi_clicked();
    void on_calibrate_clicked();
};
#endif // MAINWINDOW_H
