#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkDatagram>
#include <QTimer>
#include <QPainter>

#include "customlable.h"
#include "udp.h"
#include "checkerboardtest.h"

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
    const QImage & getImage();

private:
    Ui::MainWindow *ui;
    Udp *server;
    Checkerboardtest *boardtest;
    QTimer *timer;
    QTimer *chtimer;
    QPixmap pix;
    QImage image;

    void renderPoints(const std::vector<cv::Point2f>& points) const;

private slots:
    void hasToRender(void);

};
#endif // MAINWINDOW_H
