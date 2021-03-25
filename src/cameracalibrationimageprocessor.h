#ifndef CAMERACALIBRATIONIMAGEPROCESSOR_H
#define CAMERACALIBRATIONIMAGEPROCESSOR_H

#include <QPoint>
#include <cmath>


class CameraCalibrationImageProcessor
{
public:
    CameraCalibrationImageProcessor();
    static bool FindCorner(char cameraImage[], int width, int height, int *x, int *y, int delta);

private:

    static int GetAverage(char cameraImage[], int width, int x, int y);
};

#endif // CAMERACALIBRATIONIMAGEPROCESSOR_H
