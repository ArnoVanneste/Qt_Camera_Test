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

    int getDelta(void);
    void setDelta(const int delta);

private slots:
    void realtimeDataSlot();

    void on_fps_clicked();

    void on_delta_clicked();

private:
    Ui::FpsChart *ui;
    QTimer *dataTimer;
    QSharedPointer<QCPAxisTickerTime> timeTicker;
    int fps;
    int delta;
};
