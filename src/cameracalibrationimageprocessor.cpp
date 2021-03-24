#include "cameracalibrationimageprocessor.h"

CameraCalibrationImageProcessor::CameraCalibrationImageProcessor()
{    

}

bool CameraCalibrationImageProcessor::FindCorner(char *cameraImage, int width, int height, int *x, int *y, int delta)
{
    //When near the edge of the image, do not do the calculation
    if (((*x) < delta) || ((*y) < delta) || ((*x) >= (width - delta)) || ((*y) >= (height - delta)))
    {
        return false;
    }

    int xx = (*x) - delta;
    int yy = (*y) - delta;

    QPoint points[4];

    int dx = 1;
    int dy = 0;
    for (int kant = 0; kant < 4; kant++)
    {
        int xStart = xx;
        int yStart = yy;
        int values[2 * delta + 1];
        for (int teller = 0; teller <= 2 * delta; teller++)
        {
            values[teller] = GetAverage(cameraImage, width, xx, yy);
            xx += dx;
            yy += dy;
        }

        int average = (values[0] + values[2 * delta]) / 2;
        // FIXME in order to prevent spikes, it may be useful to do a median filtering first
        // OTOH the averaging should already have smoothened things out a bit
        int startContrastPosition = 0, endContrastPosition = 2 * delta;
        if (values[0] < values[2 * delta])
        {
            while ((startContrastPosition < 2 * delta) && (values[startContrastPosition + 1] <= average))
                ++startContrastPosition;
            while ((endContrastPosition > 0) && (values[endContrastPosition - 1] >= average))
                --endContrastPosition;
        }
        else
        {
            while ((startContrastPosition < 2 * delta) && (values[startContrastPosition + 1] >= average))
                ++startContrastPosition;
            while ((endContrastPosition > 0) && (values[endContrastPosition - 1] <= average))
                --endContrastPosition;
        }
        // FIXME this assumes that startContrastPosition and endContrastPosition are very close together
        // if they are "far" apart, it may be better to draw a straight line through them
        // and calculate the position where value=average is reached
        int minDist = INT_MAX;
        int optPosition = -1;
        for (int position = startContrastPosition; position <= endContrastPosition; ++position)
        {
            int dist = std::abs(values[position] - average);
            if (dist < minDist)
            {
                minDist = dist;
                optPosition = position;
            }
        }
        if (optPosition < startContrastPosition)
            return false;
        points[kant] = QPoint(xStart + optPosition * dx, yStart + optPosition * dy);

        // rotate direction over 90 degrees
        int help = dx;
        dx = -dy;
        dy = help;
    }

    double x0 = points[0].x();
    double y0 = points[0].y();
    double x1 = points[1].x();
    double y1 = points[1].y();
    double x2 = points[2].x();
    double y2 = points[2].y();
    double x3 = points[3].x();
    double y3 = points[3].y();

    //double x02 = X2 - X0;
    double minx02 = x0 - x2;
    double y02 = y2 - y0;
    //double x13 = X3 - X1;
    double minx13 = x1 - x3;
    double y13 = y3 - y1;
    double p0Xp2 = x0 * y2 - x2 * y0;
    double p1Xp3 = x1 * y3 - x3 * y1;

    // p is the intersection between lines p0p2 and p1p3
    // p on |p1p3| => (p-p1) || (p3-p1) => (p-p1) x (p3-p1) = 0 => p x (p3-p1) = p1 x (p3-p1) = p1 x p3
    // p on |p0p2| => (p-p0) || (p2-p0) => (p-p0) x (p2-p0) = 0 => p x (p2-p0) = p0 x (p2-p0) = p0 x p2
    // solving system:
    // x*(Y3-Y1) - (X3-X1)*y = X1*Y3-X3*Y1 => x*y13 - y*x13 = p1xp3 => y13*x + minx13*y = p1xp3
    // x*(Y2-Y0) - (X2-X0)*y = X0*Y2-X2*Y0 => x*y02 - y*x02 = p0xp2 => y02*x + minx02*y = p0xp2
    double noemer = y13 * minx02 - y02 * minx13;
    // remark: noemer is vector product p0p2 x p1p3
    // p0p2 and p1p3 are supposed to be as perpendicular as possible
    //    => sin(a) = p0p2 x p1p3 / (|p0p2| . |p1p3|) > 0.9 (for limit set to (a very forgiving) 64 degrees)
    double distP0P2 = std::sqrt(std::pow(points[2].x() - points[0].x(), 2) + std::pow(points[2].y() - points[0].y(), 2));
    double distP1P3 = std::sqrt(std::pow(points[3].x() - points[1].x(), 2) + std::pow(points[3].y() - points[1].y(), 2));
    if (std::abs(noemer) <= 0.9 * distP0P2 * distP1P3) // this condition also covers cases p1==p3 and p2==p4
        return false;

    x = (int *)(int)std::round((p1Xp3 * minx02 - p0Xp2 * minx13) / noemer);
    y = (int *)(int)std::round((y13 * p0Xp2 - y02 * p1Xp3) / noemer);

    return true;
}

int CameraCalibrationImageProcessor::GetAverage(char *cameraImage, int width, int x, int y)
{
    int position = y * width + x;
    // neem gemidelde van de de 9 pixels rond de geselecteerde pixel
    // remark: as we will be comparing these values, dividing by 9 is not strictly necessary, and is omitted
    //         in order words: we merely calculate the sum, not the average
    return (int)cameraImage[position - width - 1] + (int)cameraImage[position - width] + (int)cameraImage[position - width + 1] +
           (int)cameraImage[position - 1] + (int)cameraImage[position] + (int)cameraImage[position + 1] +
           (int)cameraImage[position + width - 1] + (int)cameraImage[position + width] + (int)cameraImage[position + width + 1];
}
