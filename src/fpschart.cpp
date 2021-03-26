#include "fpschart.h"
#include "ui_fpschart.h"

FpsChart::FpsChart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FpsChart)
{
    ui->setupUi(this);
    this->fps = 0;
    ui->mPlot->addGraph(); // blue line
    ui->mPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    this->timeTicker = QSharedPointer<QCPAxisTickerTime>(new QCPAxisTickerTime);
    this->timeTicker->setTimeFormat("%h:%m:%s");
    ui->mPlot->xAxis->setTicker(this->timeTicker);
    ui->mPlot->axisRect()->setupFullAxesBox();
    ui->mPlot->yAxis->setRange(0, 150);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->mPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->mPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->mPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->mPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    this->dataTimer = new QTimer();
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer->start(500); // Interval 0 means to refresh as fast as possible
}

FpsChart::~FpsChart()
{
    delete ui;
}

void FpsChart::realtimeDataSlot()
{
    static QElapsedTimer timer;
    ui->mPlot->graph(0)->addData(timer.elapsed()/1000, this->fps);
    ui->mPlot->xAxis->setRange(timer.elapsed()/1000, 100, Qt::AlignCenter);
    ui->mPlot->replot();
}

int FpsChart::getFps() {
    return fps;
}

void FpsChart::setFps(const int fps) {
    this->fps = fps;
}
