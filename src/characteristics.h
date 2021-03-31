#ifndef CHARACTERISTICS_H
#define CHARACTERISTICS_H

#include <vector>

class Characteristics
{
public:
    Characteristics(
        double A = 0,
        double B = 0,
        double C = 0,
        double D = 0,
        double E = 0,
        double F = 0,
        double G = 0,
        double H = 0,
        double U0 = 0,
        double V0 = 0,
        double K1 = 0,
        double K2 = 0):
        m_A(A),
        m_B(B),
        m_C(C),
        m_D(D),
        m_E(E),
        m_F(F),
        m_G(G),
        m_H(H),
        m_U0(U0),
        m_V0(V0),
        m_K1(K1),
        m_K2(K2)
    {}

    static Characteristics FirstEstimation(
        const std::vector<std::pair<double, double> >& pixel_coordinates,
        const std::vector<std::pair<double, double> >& space_coordinates,
        int width,
        int height);

    static void Solve(
        std::vector<
            std::vector<double>
        >& a,
        std::vector<double>& b);

    std::pair<double, double> ToPixelCoordinates(
        const std::pair<double, double>& space_coordinates);

    std::pair<double, double> ToSpaceCoordinates(
        const std::pair<double, double>& pixel_coordinates);

    double m_A;
    double m_B;
    double m_C;
    double m_D;
    double m_E;
    double m_F;
    double m_G;
    double m_H;
    double m_I = 1.0;
    double m_U0;
    double m_V0;
    double m_K1;
    double m_K2;
};

#endif // CHARACTERISTICS_H
