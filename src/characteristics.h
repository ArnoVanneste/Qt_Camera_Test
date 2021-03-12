#ifndef CHARACTERISTICS_H
#define CHARACTERISTICS_H

#include "opencv2/core/core.hpp"
#include <iostream>
#include <stdio.h>
#include <QPoint>


class Characteristics
{
public:
    Characteristics
    (
            double A = 0.0,
            double B = 0.0,
            double C = 0.0,
            double D = 0.0,
            double E = 0.0,
            double F = 0.0,
            double G = 0.0,
            double H = 0.0,
            double I = 0.0,
            double U0 = 0.0,
            double V0 = 0.0,
            double K1 = 0.0,
            double K2 = 0.0
    ):
        A(A),
        B(B),
        C(C),
        D(D),
        E(E),
        F(F),
        G(G),
        H(H),
        I(I),
        U0(U0),
        V0(V0),
        K1(K1),
        K2(K2)
    {}

    static Characteristics FirstEstimation(std::vector<cv::Point2f> pixelCoordinates, std::vector<cv::Point2f> spaceCoordinates, int width, int height);
    QPoint ToSpaceCoordinates(QPoint pixelCoordinates);
    QPoint ToPixelCoordinates(QPoint spaceCoordinates);


public:
    double A;
    double B;
    double C;
    double D;
    double E;
    double F;
    double G;
    double H;
    double I = 1.0;
    double U0;
    double V0;
    double K1;
    double K2;

private:

    static void Solve(std::vector<std::vector<double> > a, std::vector<double> b);

};

#endif // CHARACTERISTICS_H
