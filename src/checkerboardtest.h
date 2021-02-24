#ifndef CHECKERBOARDTEST_H
#define CHECKERBOARDTEST_H

#include <QObject>
#include <QImage>
#include <QFile>
#include <QTextStream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>


class Checkerboardtest : public QObject
{

    Q_OBJECT

public:
    Checkerboardtest();

    std::vector<cv::Point2f> calc(const QImage& img);

    cv::Mat qimage_to_mat(const QImage& img, int format);

private:
    int CHECKERBOARD[2];
    QImage *image;

    std::vector<std::vector<cv::Point3f> > objpoints;
    std::vector<std::vector<cv::Point2f> > imgpoints;
    std::vector<cv::Point3f> objp;

};

#endif // CHECKERBOARDTEST_H
