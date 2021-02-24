#include "checkerboardtest.h"

Checkerboardtest::Checkerboardtest()
{

    this->CHECKERBOARD[0] = 6;
    this->CHECKERBOARD[1] = 11;

}

std::vector<cv::Point2f> Checkerboardtest::calc(const QImage& image) {

    std::vector<std::vector<cv::Point3f> > objpoints;
    std::vector<std::vector<cv::Point2f> > imgpoints;

    std::vector<cv::Point3f> objp;
    for(int i{0}; i<CHECKERBOARD[1]; i++)
    {
      for(int j{0}; j<CHECKERBOARD[0]; j++)
        objp.push_back(cv::Point3f(j,i,0));
    }

    cv::Mat gray, frame;
    // vector to store the pixel coordinates of detected checker board corners
    std::vector<cv::Point2f> corner_pts;

    frame = qimage_to_mat(image, CV_8U);

    bool success = cv::findChessboardCorners(
                    frame,
                    cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]),
                    corner_pts,
                    cv::CALIB_CB_ADAPTIVE_THRESH |
//                    cv::CALIB_CB_FAST_CHECK |
                    cv::CALIB_CB_NORMALIZE_IMAGE);

    if(success)
    {

        cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);

        // refining pixel coordinates for given 2d points.
        cv::cornerSubPix(frame,corner_pts,cv::Size(11,11), cv::Size(-1,-1),criteria);

        objpoints.push_back(objp);
        imgpoints.push_back(corner_pts);

        cv::Mat cameraMatrix,distCoeffs,R,T;

        cv::calibrateCamera(objpoints, imgpoints, cv::Size(frame.rows,frame.cols), cameraMatrix, distCoeffs, R, T);

        std::ostringstream oss;
        oss << "cameraMatrix = " << std::endl << " " << cameraMatrix << std::endl;
        QString cMatrix(oss.str().c_str());

        oss << "distCoeffs = " << std::endl << " " << distCoeffs << std::endl;
        QString dCoeffs(oss.str().c_str());

        oss << "Rotation vector = " << std::endl << " " << R << std::endl;
        QString r(oss.str().c_str());

        oss << "Translation vector = " << std::endl << " " << T << std::endl;
        QString t(oss.str().c_str());

        QString filename="D:\\repos\\Qt_Camera_Test\\src\\Data.txt";
        QFile file( filename );
        if ( file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &file );
            stream << t << Qt::endl;
        }

        return corner_pts;

    }

}

cv::Mat Checkerboardtest::qimage_to_mat(const QImage &img, int format) {
    return cv::Mat(img.height(), img.width(), format,
                       const_cast<uchar*>(img.bits()),
                       img.bytesPerLine()).clone();
}
