#include "customlable.h"
Customlable::Customlable(QWidget *parent)
    : QLabel(parent)
{
    drawPoints = false;
    showRoi = false;
    DEFAULT_WIDTH = 640;
    DEFAULT_HEIGHT = 512;
    roi = QRect(((1280 - DEFAULT_WIDTH) / 2), ((1024 - DEFAULT_HEIGHT) / 2), DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

void Customlable::setPoints(const std::vector<cv::Point2f>& points) {

    this->points = points;

}

void Customlable::paintEvent(QPaintEvent *event) {

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

//    if(points.size() != 0 ) {

//        for(unsigned int i = 0; i < points.size(); i++) {
//            if(points[0].x < points[i].x) {
//                points[0].x = points[i].x;
//            }
//        }

//        for(unsigned int i = 0; i < points.size(); i++) {
//            if(points[0].y < points[i].y) {
//                points[0].y = points[i].y;
//            }
//        }

//        int x = (int)points[0].x;
//        int y = (int)points[0].y;

//        QPoint topLeft;

//        topLeft.setX(x);
//        topLeft.setY(y);

//        for(unsigned int i = 0; i < points.size(); i++) {
//            if(points[0].x > points[i].x) {
//                points[0].x = points[i].x;
//            }
//        }

//        for(unsigned int i = 0; i < points.size(); i++) {
//            if(points[0].y > points[i].y) {
//                points[0].y = points[i].y;
//            }
//        }

//        x = (int)points[0].x;
//        y =(int)points[0].y;

//        QPoint bottomRight;

//        bottomRight.setX(x);
//        bottomRight.setY(y);

//        QRect roi(topLeft, bottomRight);

//        paint.drawRect(roi);

//    }

    if(this->showRoi) {
        paint.drawRect(roi);
    }

    paint.end();

}

void Customlable::setWidth(int width) {
    this->roi.setWidth(width);
//    this->roi.moveCenter(QPoint(320, 256));
    this->update();
}

int Customlable::getWidth(void) {
    return this->roi.width();
}

void Customlable::setHeight(int height) {
    this->roi.setHeight(height);
//    this->roi.moveCenter(QPoint(320, 256));
    this->update();
}

int Customlable::getHeight(void) {
    return this->roi.height();
}

void Customlable::center(void) {

    const unsigned short width = this->roi.width();
    const unsigned short height = this->roi.height();

    this->roi.setRect((1280 - width) / 2, (1024 - height) / 2, width, height);
}

void Customlable::setRoiRect(QRect roi) {
    this->roi = roi;
}

QRect Customlable::getRoiRect(void) {
    return this->roi;
}

