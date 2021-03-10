#include <QDialog>
#include "qcustomplot.h"
#include "axistag.h"

namespace Ui {
class FpsChart;
}

class FpsChart : public QDialog
{
    Q_OBJECT

public:
    explicit FpsChart(QWidget *parent = nullptr);
    ~FpsChart();

    int getFps();
    void setFps(const int fps);

private slots:
    void realtimeDataSlot();

private:
    Ui::FpsChart *ui;
    QTimer *dataTimer;
    QSharedPointer<QCPAxisTickerTime> timeTicker;
    int fps;
};
