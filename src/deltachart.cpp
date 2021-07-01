#include "deltachart.h"
#include "ui_deltachart.h"

DeltaChart::DeltaChart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeltaChart)
{
    ui->setupUi(this);
    this->delta = 0;
    ui->vPlot->addGraph();
    ui->vPlot->graph(0)->setPen(QPen(QColor(255, 0, 0)));

    this->timeTicker = QSharedPointer<QCPAxisTickerTime>(new QCPAxisTickerTime);
    this->timeTicker->setTimeFormat("%m:%s");
    ui->vPlot->xAxis->setTicker(this->timeTicker);
    ui->vPlot->axisRect()->setupFullAxesBox();
    ui->vPlot->yAxis->setRange(10, 50);

    connect(ui->vPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->vPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->vPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->vPlot->yAxis2, SLOT(setRange(QCPRange)));

    this->deltaTimer = new QTimer();
    connect(deltaTimer, SIGNAL(timeout()), this, SLOT(realtimeDeltaSlot()));
    deltaTimer->start(10); // Interval 0 means to refresh as fast as possible
}

DeltaChart::~DeltaChart()
{
    delete ui;
}

void DeltaChart::realtimeDeltaSlot() {
    static QElapsedTimer timer;
    ui->vPlot->graph(0)->addData(timer.elapsed()/1000, this->delta);
    ui->vPlot->xAxis->setRange(timer.elapsed()/1000, 100, Qt::AlignCenter);
    ui->vPlot->replot();
}

int DeltaChart::getDelta() {
    return delta;
}

void DeltaChart::setDelta(const int delta) {
    this->delta = delta;
}
