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

    Customlable(QWidget *parent);

    ~Customlable()
    {}

public:
    void setPoints(const std::vector<cv::Point2f>& points);

    void setWidth(int width);
    int getWidth(void);

    void setHeight(int height);
    int getHeight(void);

    void center(void);
    void setRoiRect(QRect roi);
    QRect getRoiRect(void);
    QRect roi;

    bool drawPoints;
    bool showRoi;

protected:
    void paintEvent(QPaintEvent *event);

private:
    std::vector<cv::Point2f> points;    
    int DEFAULT_WIDTH;
    int DEFAULT_HEIGHT;
};

#endif // CUSTOMLABLE_H
