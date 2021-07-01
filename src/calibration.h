#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <characteristics.h>

#include <vector>

struct CalibrationGridParameters {
    double grid_size = 20;
    int num_rows = 5;
    int num_columns = 10;
};

class Calibration
{
public:
    Calibration();

    static bool FindCorner(
        char *camera_image,
        int width,
        int height,
        int& x,
        int& y,
        int delta);

    static int GetAverage(
        char *camera_image,
        int width,
        int x,
        int y);

    static double Distance(
        const std::pair<double, double>& p1,
        const std::pair<double, double>& p2);

    static void Evaluate(
        const double *parameters,
        int mdata,
        const void *data,
        double *fvec,
        int *userbreak);

    bool Calibrate(
        char *camera_image,
        int width,
        int height,
        std::vector<
            std::pair<double, double>
        > corner_points,
        int search_radius);

    void Reset(void);

    bool corner_1_found = false;
    bool corner_2_found = false;
    bool corner_3_found = false;
    bool corner_4_found = false;
    bool grid_calculated = false;
    CalibrationGridParameters calc_grid;
    static std::vector<std::pair<double, double> > m_calibration_space_data;
    static std::vector<std::pair<double, double> > m_calibration_pixel_data;
    Characteristics m_optimized_characteristics;

    static unsigned num_samples;
};

#endif // CALIBRATION_H
