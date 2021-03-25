#include "cablibrate.h"

QPoint* Cablibrate::_calibrationPixelData;
QPoint* Cablibrate::_calibrationSpaceData;
int Cablibrate::numSamples;

void Cablibrate::Reset(void)
{
    GridCalculated = false;
    Corner1Found = false;
    Corner2Found = false;
    Corner3Found = false;
    Corner4Found = false;
}

bool Cablibrate::Calibrate(char* cameraImage, int width, int height, std::vector<cv::Point2f> cornerpoints, int searchRadius)
{
    Reset();

    // approximate location of the corners, as indicated by the user
    int u1 = cornerpoints[0].x;
    int v1 = cornerpoints[0].y;
    int u2 = cornerpoints[1].x;
    int v2 = cornerpoints[1].y;
    int u3 = cornerpoints[2].x;
    int v3 = cornerpoints[2].y;
    int u4 = cornerpoints[3].x;
    int v4 = cornerpoints[3].y;

    std::vector<cv::Point2f> pixelCoords = {
        cv::Point2f(u1, v1),
        cv::Point2f(u2, v2),
        cv::Point2f(u3, v3),
        cv::Point2f(u4, v4)
    };

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

    Cablibrate::_calibrationSpaceData = new QPoint[(CalGrid.NumRows + 1) * (CalGrid.NumColumns + 1)];
    Cablibrate::_calibrationPixelData = new QPoint[sizeof(_calibrationSpaceData)];

    Cablibrate::numSamples = 0;
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
            Cablibrate::_calibrationSpaceData[numSamples] = space;
            Cablibrate::_calibrationPixelData[numSamples] = foundPixel;
            Cablibrate::numSamples++;
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

    OptimizationResult t = solver.Solve(evaluate, parameters, Cablibrate::numSamples);
    Characteristics Characteristics
    {
        t.OptimizedParameters[0],
        t.OptimizedParameters[1],
        t.OptimizedParameters[2],
        t.OptimizedParameters[3],
        t.OptimizedParameters[4],
        t.OptimizedParameters[5],
        t.OptimizedParameters[6],
        t.OptimizedParameters[7],
        t.OptimizedParameters[8],
        t.OptimizedParameters[9],
        t.OptimizedParameters[10],
        t.OptimizedParameters[11]
    };

    GridCalculated = true;

    return true;

}

void Cablibrate::evaluate(const double* par, int m_dat, const void *data, double *fvec, int *userbreak) {
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
    for (int teller = 0; teller < Cablibrate::numSamples; teller++)
    {
        QPoint calcPixel = loopCharacteristics.ToPixelCoordinates(Cablibrate::_calibrationSpaceData[teller]);
        QPair<QPoint, QPoint> diff;
        diff.first = calcPixel;
        diff.second = Cablibrate::_calibrationPixelData[teller];
        fvec[teller] = sqrt(pow((diff.first.x() - diff.second.x()), 2) + pow((diff.first.y() - diff.second.y()), 2));
    }
}
