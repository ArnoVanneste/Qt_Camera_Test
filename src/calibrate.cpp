#include "calibrate.h"

QPoint* Calibrate::_calibrationPixelData;
QPoint* Calibrate::_calibrationSpaceData;
int Calibrate::numSamples;

void Calibrate::Reset(void)
{
    GridCalculated = false;
    Corner1Found = false;
    Corner2Found = false;
    Corner3Found = false;
    Corner4Found = false;
}

std::vector<cv::Point2f> Calibrate::FindCorners (std::vector<cv::Point2f> points) {
    std::vector<cv::Point2f> result;
    result.push_back(points[0]);
    result.push_back(points[CalGrid.NumColumns - 1]);
    result.push_back(points[CalGrid.NumColumns * (CalGrid.NumRows - 1)]);
    result.push_back(points[(CalGrid.NumColumns * CalGrid.NumRows) - 1]);

    return result;
}

bool Calibrate::Calibration(char* cameraImage, int width, int height, std::vector<cv::Point2f> cornerpoints, int searchRadius)
{
    Reset();

    std::vector<cv::Point2f> pixelCoords = FindCorners(cornerpoints);

    double targetLeftPosition = -0.5 * CalGrid.NumColumns * CalGrid.GridSize;
    double targetRightPosition = 0.5 * CalGrid.NumColumns * CalGrid.GridSize;
    double targetBottomPosition = CalGrid.GridSize;
    double targetTopPosition = (CalGrid.NumRows + 1) * CalGrid.GridSize;

    std::vector<cv::Point2f> spaceCoords = {
        cv::Point2f(targetLeftPosition, targetBottomPosition),
        cv::Point2f(targetRightPosition, targetBottomPosition),
        cv::Point2f(targetLeftPosition, targetTopPosition),
        cv::Point2f(targetRightPosition, targetTopPosition)
    };

    Characteristics estimated = Characteristics::FirstEstimation(pixelCoords, spaceCoords, width, height);

    Calibrate::_calibrationSpaceData = new QPoint[(CalGrid.NumRows + 1) * (CalGrid.NumColumns + 1)];
    Calibrate::_calibrationPixelData = new QPoint[sizeof(_calibrationSpaceData)];

    Calibrate::numSamples = 0;
    for (int row = 0; row <= CalGrid.NumRows; ++row)
    {
        double y = (row + 1) * CalGrid.GridSize;
        for (int column = 0; column <= CalGrid.NumColumns; ++column)
        {
            double x = (column - 0.5 * CalGrid.NumColumns) * CalGrid.GridSize;
            QPoint space(x, y);
            QPoint estPixel = estimated.ToPixelCoordinates(space);
            int u = estPixel.x();
            int v = estPixel.y();
            CameraCalibrationImageProcessor::FindCorner(cameraImage, width, height, &u, &v, searchRadius);
            QPoint foundPixel(u, v);
            Calibrate::_calibrationSpaceData[numSamples] = space;
            Calibrate::_calibrationPixelData[numSamples] = foundPixel;
            Calibrate::numSamples++;
        }
    }

    double parameters[12];
    parameters[0] = estimated.A;
    parameters[1] = estimated.B;
    parameters[2] = estimated.C;
    parameters[3] = estimated.D;
    parameters[4] = estimated.E;
    parameters[5] = estimated.F;
    parameters[6] = estimated.G;
    parameters[7] = estimated.H;
    parameters[8] = estimated.U0;
    parameters[9] = estimated.V0;
    parameters[10] = estimated.K1;
    parameters[11] = estimated.K2;

    LMSolver solver(1E-20, 1E-20, 1E-20);

    OptimizationResult* t = solver.Solve(evaluate, parameters, Calibrate::numSamples);
    this->characteristics =
    {
        t->OptimizedParameters[0],
        t->OptimizedParameters[1],
        t->OptimizedParameters[2],
        t->OptimizedParameters[3],
        t->OptimizedParameters[4],
        t->OptimizedParameters[5],
        t->OptimizedParameters[6],
        t->OptimizedParameters[7],
        t->OptimizedParameters[8],
        t->OptimizedParameters[9],
        t->OptimizedParameters[10],
        t->OptimizedParameters[11]
    };

    delete t;

    GridCalculated = true;

    return true;

}

void Calibrate::evaluate(const double* par, int m_dat, const void *data, double *fvec, int *userbreak) {
    Characteristics loopCharacteristics
    {
        par[0],
        par[1],
        par[2],
        par[3],
        par[4],
        par[5],
        par[6],
        par[7],
        par[8],
        par[9],
        par[10],
        par[11]
    };
    for (int teller = 0; teller < Calibrate::numSamples; teller++)
    {
        QPoint calcPixel = loopCharacteristics.ToPixelCoordinates(Calibrate::_calibrationSpaceData[teller]);
        QPair<QPoint, QPoint> diff;
        diff.first = calcPixel;
        diff.second = Calibrate::_calibrationPixelData[teller];
        fvec[teller] = sqrt(pow((diff.first.x() - diff.second.x()), 2) + pow((diff.first.y() - diff.second.y()), 2));
    }
}

Characteristics Calibrate::getParams(void) const {
    return this->characteristics;
}
