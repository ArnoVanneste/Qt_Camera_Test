#ifndef CUSTOMLABLE_H
#define CUSTOMLABLE_H

#include<QPainter>
#include<QWidget>
#include<QLabel>

#include "opencv2/core/core.hpp"

class Customlable : public QLabel
{

    Q_OBJECT

public:

    Customlable(QWidget *parent)
        : QLabel(parent),
          drawPoints(false)
    {}

    ~Customlable()
    {}

public:
    void setPoints(const std::vector<cv::Point2f>& points);
    bool drawPoints;

protected:
    void paintEvent(QPaintEvent *event);

private:
    std::vector<cv::Point2f> points;
};

#endif // CUSTOMLABLE_H
