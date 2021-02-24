#include "customlable.h"

void Customlable::setPoints(const std::vector<cv::Point2f>& points) {

    this->points = points;

}

void Customlable::paintEvent(QPaintEvent *event)
{

    QLabel::paintEvent(event);

    QPainter paint;
    paint.begin(this);
    paint.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));

    if (this->drawPoints) {
        for (unsigned index = 0; index < this->points.size(); index++) {
            const unsigned x = this->points[index].x;
            const unsigned y = this->points[index].y;
            paint.drawEllipse(QRectF(x, y, 5, 5));
        }
    }

    paint.end();

}

