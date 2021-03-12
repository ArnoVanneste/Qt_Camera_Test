#include "cablibrate.h"

Cablibrate::Cablibrate()
{

}

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


}
