#include "calibration.h"

#include <globals.h>
#include <lmsolver.h>
#include <helpers.h>

#include <QThread>

#include <cstdlib>
#include <cmath>

#include <QDebug>

std::vector<std::pair<double, double> > Calibration::m_calibration_pixel_data;
std::vector<std::pair<double, double> > Calibration::m_calibration_space_data;

unsigned Calibration::num_samples = 0;

Calibration::Calibration()
{

}

bool Calibration::FindCorner(
    char *camera_image,
    int width,
    int height,
    int& x,
    int& y,
    int delta)
{
    //When near the edge of the image, do not do the calculation
    if ((x < delta) || (y < delta) || (x >= (width - delta)) || (y >= (height - delta)))
    {
        return false;
    }

    int xx = x - delta;
    int yy = y - delta;

    std::vector<std::pair<double, double> > points (4);

    int dx = 1;
    int dy = 0;
    for (int kant = 0; kant < 4; kant++)
    {
        int x_start = xx;
        int y_start = yy;
        int* values = (int *)malloc(sizeof(int) * (2 * delta + 1));
        for (int teller = 0; teller <= 2 * delta; teller++)
        {
            values[teller] = GetAverage(camera_image, width, xx, yy);
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
        int min_dist = 0x7FFFFFFF;
        int opt_position = -1;
        for (int position = startContrastPosition; position <= endContrastPosition; ++position)
        {
            int dist = std::abs(values[position] - average);
            if (dist < min_dist)
            {
                min_dist = dist;
                opt_position = position;
            }
        }
        if (opt_position < startContrastPosition)
            return false;
        points[kant] = std::make_pair(x_start + opt_position * dx, y_start + opt_position * dy);

        // rotate direction over 90 degrees
        int help = dx;
        dx = -dy;
        dy = help;
    }

    double x0 = points[0].first;
    double y0 = points[0].second;
    double x1 = points[1].first;
    double y1 = points[1].second;
    double x2 = points[2].first;
    double y2 = points[2].second;
    double x3 = points[3].first;
    double y3 = points[3].second;

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
    double dist_p0_p2 = Distance(points[0], points[2]);
    double dist_p1_p3 = Distance(points[1], points[3]);
    if (std::abs(noemer) <= 0.9 * dist_p0_p2 * dist_p1_p3) // this condition also covers cases p1==p3 and p2==p4
        return false;

    x = (int)std::round((p1Xp3 * minx02 - p0Xp2 * minx13) / noemer);
    y = (int)std::round((y13 * p0Xp2 - y02 * p1Xp3) / noemer);

    return true;
}

int Calibration::GetAverage(
    char *camera_image,
    int width,
    int x,
    int y)
{
    int position = y * width + x;
    // neem gemidelde van de de 9 pixels rond de geselecteerde pixel
    // remark: as we will be comparing these values, dividing by 9 is not strictly necessary, and is omitted
    //         in order words: we merely calculate the sum, not the average
    return (int)camera_image[position - width - 1] + (int)camera_image[position - width] + (int)camera_image[position - width + 1] +
           (int)camera_image[position - 1] + (int)camera_image[position] + (int)camera_image[position + 1] +
           (int)camera_image[position + width - 1] + (int)camera_image[position + width] + (int)camera_image[position + width + 1];
}

double Calibration::Distance(
    const std::pair<double, double>& p1,
    const std::pair<double, double>& p2)
{
    double dx = p1.first - p2.first;
    double dy = p1.second - p2.second;
    if (dx == 0.0)
        return std::abs(dy);
    if (dy == 0.0)
        return std::abs(dx);

    return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}

void Calibration::Evaluate(
    const double *parameters,
    int mdata,
    const void *data,
    double *fvec,
    int *userbreak)
{
    Characteristics loop_characteristics(
        parameters[0],
        parameters[1],
        parameters[2],
        parameters[3],
        parameters[4],
        parameters[5],
        parameters[6],
        parameters[7],
        parameters[8],
        parameters[9],
        parameters[10],
        parameters[11]
    );

    for (unsigned index = 0; index < Globals::num_samples; index++) {
        std::pair<double, double> calc_pixel = loop_characteristics.ToPixelCoordinates(m_calibration_space_data[index]);
        fvec[index] = Distance(calc_pixel, m_calibration_pixel_data[index]);
    }

}

bool Calibration::Calibrate(
    char *camera_image,
    int width,
    int height,
    std::vector<
        std::pair<double, double>
    > corner_points,
    int search_radius)
{
    qDebug() << "Calibration Thread id: " << QThread::currentThreadId();

    this->Reset();

    // approximate location of the corners, as indicated by the user
    int u1 = corner_points[0].first;
    int v1 = corner_points[0].second;
    int u2 = corner_points[1].first;
    int v2 = corner_points[1].second;
    int u3 = corner_points[2].first;
    int v3 = corner_points[2].second;
    int u4 = corner_points[3].first;
    int v4 = corner_points[3].second;

    corner_1_found = Calibration::FindCorner(camera_image, width, height, u1, v1, search_radius);
    corner_2_found = Calibration::FindCorner(camera_image, width, height, u2, v2, search_radius);
    corner_3_found = Calibration::FindCorner(camera_image, width, height, u3, v3, search_radius);
    corner_4_found = Calibration::FindCorner(camera_image, width, height, u4, v4, search_radius);

    if (corner_1_found && corner_2_found &&
        corner_3_found && corner_4_found)
    {
        std::vector<std::pair<double, double> > pixel_coords {
            {u1, v1},
            {u2, v2},
            {u3, v3},
            {u4, v4}
        };

        // the corresponding points on the calibration target in space coordinates
        double target_left_position = -0.5 * this->calc_grid.num_columns * this->calc_grid.grid_size;
        double target_right_position = 0.5 * this->calc_grid.num_columns * this->calc_grid.grid_size;
        double target_bottom_position = this->calc_grid.grid_size;
        double target_top_position = (this->calc_grid.num_rows + 1) * this->calc_grid.grid_size;

        std::vector<std::pair<double, double> > space_coords {
            {target_left_position, target_bottom_position},
            {target_right_position, target_bottom_position},
            {target_left_position, target_top_position},
            {target_right_position, target_top_position}
        };

        Characteristics estimated = Characteristics::FirstEstimation(pixel_coords, space_coords, width, height);
        // m_calibration_space_data.reserve((this->calc_grid.num_rows + 1) * (this->calc_grid.num_columns + 1));
        // m_calibration_pixel_data.reserve(m_calibration_space_data.capacity());

        m_calibration_space_data.clear();
        m_calibration_pixel_data.clear();
        num_samples = 0;

        //generate a full virtual replica of the calibration board
        for (int row = 0; row <= this->calc_grid.num_rows; ++row)
        {
            double y = (row + 1) * this->calc_grid.grid_size;
            for (int column = 0; column <= this->calc_grid.num_columns; ++column)
            {
                double x = (column - 0.5 * this->calc_grid.num_columns) * this->calc_grid.grid_size;
                std::pair<double, double> space {x, y};
                std::pair<double, double> est_pixel = estimated.ToPixelCoordinates(space);
                int u = (int)est_pixel.first;
                int v = (int)est_pixel.second;
                Calibration::FindCorner(camera_image, width, height, u, v, search_radius);
                std::pair<double, double> found_pixel {u, v};
                m_calibration_space_data.push_back(space);
                m_calibration_pixel_data.push_back(found_pixel);
                num_samples++;
            }
        }

        double* parameters = (double *)malloc(sizeof(double) * 12);
        parameters[0] = estimated.m_A;
        parameters[1] = estimated.m_B;
        parameters[2] = estimated.m_C;
        parameters[3] = estimated.m_D;
        parameters[4] = estimated.m_E;
        parameters[5] = estimated.m_F;
        parameters[6] = estimated.m_G;
        parameters[7] = estimated.m_H;
        parameters[8] = estimated.m_U0;
        parameters[9] = estimated.m_V0;
        parameters[10] = estimated.m_K1;
        parameters[11] = estimated.m_K2;

        LMSolver solver(1e-20, 1e-20, 1e-20);
        OptimizationResult optimized_result = solver.Solve(Evaluate, parameters, num_samples);
        m_optimized_characteristics = Characteristics(
            optimized_result.m_optimized_parameters[0],
            optimized_result.m_optimized_parameters[1],
            optimized_result.m_optimized_parameters[2],
            optimized_result.m_optimized_parameters[3],
            optimized_result.m_optimized_parameters[4],
            optimized_result.m_optimized_parameters[5],
            optimized_result.m_optimized_parameters[6],
            optimized_result.m_optimized_parameters[7],
            optimized_result.m_optimized_parameters[8],
            optimized_result.m_optimized_parameters[9],
            optimized_result.m_optimized_parameters[10],
            optimized_result.m_optimized_parameters[11]
        );

        this->grid_calculated = true;

        return true;
    }

    return false;
}

void Calibration::Reset(void)
{
    this->corner_1_found = false;
    this->corner_2_found = false;
    this->corner_3_found = false;
    this->corner_4_found = false;
    this->grid_calculated = false;
}
