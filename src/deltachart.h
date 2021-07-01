#include "qcustomplot.h"
#include "axistag.h"
#include <QDialog>

namespace Ui {
class DeltaChart;
}

class DeltaChart : public QDialog
{
    Q_OBJECT

public:
    explicit DeltaChart(QWidget *parent = nullptr);
    ~DeltaChart();

    int getDelta(void);
    void setDelta(const int delta);

private slots:
    void realtimeDeltaSlot();

private:
    Ui::DeltaChart *ui;
    QTimer *deltaTimer;
    QSharedPointer<QCPAxisTickerTime> timeTicker;
    int delta;

};
