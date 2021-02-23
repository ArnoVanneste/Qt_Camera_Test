#include "checkerboardtest.h"

Checkerboardtest::Checkerboardtest()
{

    this->CHECKERBOARD[0] = 6;
    this->CHECKERBOARD[1] = 11;

}

std::vector<cv::Point2f> Checkerboardtest::calc(QImage image) {

    cv::Mat gray, frame;
    // vector to store the pixel coordinates of detected checker board corners
    std::vector<cv::Point2f> corner_pts;

    frame = qimage_to_mat(image, CV_8U);

    bool success = cv::findChessboardCorners(
                    frame,
                    cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]),
                    corner_pts,
                    cv::CALIB_CB_ADAPTIVE_THRESH |
                    cv::CALIB_CB_FAST_CHECK |
                    cv::CALIB_CB_NORMALIZE_IMAGE);

    if(success)
    {

        cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);

        // refining pixel coordinates for given 2d points.
        cv::cornerSubPix(frame,corner_pts,cv::Size(11,11), cv::Size(-1,-1),criteria);

        return corner_pts;

    }

}

cv::Mat Checkerboardtest::qimage_to_mat(QImage &img, int format) {
    return cv::Mat(img.height(), img.width(), format,
                       const_cast<uchar*>(img.bits()),
                       img.bytesPerLine()).clone();
}
