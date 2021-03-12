#ifndef CABLIBRATE_H
#define CABLIBRATE_H

#include "opencv2/core/core.hpp"
#include <iostream>
#include <stdio.h>
#include <QPoint>

#include "characteristics.h"
#include "calibrationgridparameters.h"


class Cablibrate
{
public:
    Cablibrate();

    bool Calibrate(char* cameraImage, int width, int height, std::vector<cv::Point2f> cornerpoints, int searchRadius);
    void Reset(void);

public:
    Characteristics Characteristics;
    CalibrationGridParameters CalGrid;

    bool GridCalculated = false;

    bool Corner1Found = false;
    bool Corner2Found = false;
    bool Corner3Found = false;
    bool Corner4Found = false;

private:
    QPoint* _calibrationPixelData;
    QPoint* _calibrationSpaceData;



};

#endif // CABLIBRATE_H
