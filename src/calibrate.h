#ifndef CALIBRATE_H
#define CALIBRATE_H

#include "opencv2/core/core.hpp"
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <stdio.h>
#include <QPoint>
#include <cmath>
#include <QPair>

#include "characteristics.h"
#include "calibrationgridparameters.h"
#include "cameracalibrationimageprocessor.h"
#include "lmsolver.h"


class Calibrate
{
public:
    bool Calibration(char* cameraImage, int width, int height, std::vector<cv::Point2f> cornerpoints, int searchRadius);
    void Reset(void);

public:
    Characteristics characteristics;
    CalibrationGridParameters CalGrid;

    std::vector<cv::Point2f> FindCorners (std::vector<cv::Point2f> points);

    bool GridCalculated = false;

    bool Corner1Found = false;
    bool Corner2Found = false;
    bool Corner3Found = false;
    bool Corner4Found = false;

    static void evaluate(const double* par, int m_dat, const void *data, double *fvec, int *userbreak);

    static QPoint* _calibrationPixelData;
    static QPoint* _calibrationSpaceData;

    static int numSamples;

    Characteristics getParams(void) const;

};

#endif // CALIBRATE_H
