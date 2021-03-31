#include "characteristics.h"

#include <cassert>

Characteristics Characteristics::FirstEstimation(
    const std::vector<std::pair<double, double> >& pixel_coordinates,
    const std::vector<std::pair<double, double> >& space_coordinates,
    int width,
    int height)
{
    assert((pixel_coordinates.size() == 4) && (space_coordinates.size() == 4));

    double X1 = space_coordinates[0].first;
    double Y1 = space_coordinates[0].second;
    double X2 = space_coordinates[1].first;
    double Y2 = space_coordinates[1].second;
    double X3 = space_coordinates[2].first;
    double Y3 = space_coordinates[2].second;
    double X4 = space_coordinates[3].first;
    double Y4 = space_coordinates[3].second;

    double U1 = pixel_coordinates[0].first;
    double V1 = pixel_coordinates[0].second;
    double U2 = pixel_coordinates[1].first;
    double V2 = pixel_coordinates[1].second;
    double U3 = pixel_coordinates[2].first;
    double V3 = pixel_coordinates[2].second;
    double U4 = pixel_coordinates[3].first;
    double V4 = pixel_coordinates[3].second;

    Characteristics c(
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.5 * width,
        0.5 * height,
        0.0,
        0.0
    );

    // with K1 == K2 == 0: U=u and V=v
    // u = (Ax+By+C) / (Gx+Hy+I) => Ax + By + C - Gux - Huy - Iu = 0
    // v = (Dx+Ey+F) / (Gx+Hy+I) => Dx + Ey + F - Gvx - Hvy - Iv = 0
    // Solve for A,B,C,D,E,F,G,H,I in the case of ((U1,V1),(X1,Y1)), ..., ((U4,V4),(X4,Y4))
    // infinite number of solutions possible => without losing generality we can set I = 1.0
    c.m_I = 1.0;
    // => we end up with 8 linear equations with 8 unknowns
    std::vector<std::vector<double> > a {
        { X1, Y1, 1.0, 0.0, 0.0, 0.0, -U1 * X1, -U1 * Y1 },
        { 0.0, 0.0, 0.0, X1, Y1, 1.0, -V1 * X1, -V1 * Y1 },
        { X2, Y2, 1.0, 0.0, 0.0, 0.0, -U2 * X2, -U2 * Y2 },
        { 0.0, 0.0, 0.0, X2, Y2, 1.0, -V2 * X2, -V2 * Y2 },
        { X3, Y3, 1.0, 0.0, 0.0, 0.0, -U3 * X3, -U3 * Y3 },
        { 0.0, 0.0, 0.0, X3, Y3, 1.0, -V3 * X3, -V3 * Y3 },
        { X4, Y3, 1.0, 0.0, 0.0, 0.0, -U4 * X4, -U4 * Y4 },
        { 0.0, 0.0, 0.0, X4, Y4, 1.0, -V4 * X4, -V4 * Y4 }
    };

    std::vector<double> b = { U1, V1, U2, V2, U3, V3, U4, V4 };
    Characteristics::Solve(a, b);

    c.m_A = b[0];
    c.m_B = b[1];
    c.m_C = b[2];
    c.m_D = b[3];
    c.m_E = b[4];
    c.m_F = b[5];
    c.m_G = b[6];
    c.m_H = b[7];

    return c;
}

void Characteristics::Solve(
    std::vector<
        std::vector<double>
    >& a,
    std::vector<double>& b)
{
    const unsigned int aantal = b.size();

    assert(a.size() == aantal);
    for (int row = 0; row < aantal; ++row)
        assert(a[row].size() == aantal);

    assert(a.size() == aantal);
    std::vector<bool> used (aantal);

    for (unsigned x = 0; x < aantal; x++)
    {
        int alt = -1;
        while ((a[x][x] == 0) && (alt < aantal))
        {
            do
            {
                alt++;
            } while ((used[alt]) || (alt == x));

            if (alt < aantal)
            {
                //switchen van rij x met rij alt
                std::vector<double> d (aantal);
                d = a[x];
                a[x] = a[alt];
                a[alt] = d;

                double dd = b[x];
                b[x] = b[alt];
                b[alt] = dd;
            }
        }
        if (a[x][x] != 0)
        {
            used[x] = true;
            for (int y = 0; y < aantal; y++)
            {
                if (y != x)
                {
                    double factor = a[y][x] / a[x][x];
                    for (int z = 0; z < aantal; z++)
                    {
                        a[y][z] -= a[x][z] * factor;
                    }
                    b[y] -= b[x] * factor;
                }
            }
        }
    }
    for (unsigned x = 0; x < aantal; x++)
    {
        for (int y = 0; y < aantal; y++)
        {
            if (y != x)
            {
                b[x] -= a[x][y];
                a[x][y] = 0;
            }
        }
        if (a[x][x] != 0)
        {
            b[x] /= a[x][x];
            a[x][x] /= a[x][x];
        }
        else
            b[x] = 1;
    }
}

std::pair<double, double> Characteristics::ToPixelCoordinates(
    const std::pair<double, double>& space_coordinates)
{
    double x = space_coordinates.first;
    double y = space_coordinates.second;
    // see S:\Systemen\PORTIO\Algemeen\docs\camera calibration.pdf
    // basic transform from space coords (x,y) to pixel coords (U,V):
    // u = (Ax+By+C) / (Gx+Hy+I)
    // v = (Dx+Ey+F) / (Gx+Hy+I)
    // U = u + f*du
    // V = v + f*dv
    // with: du = u - U0
    //       dv = v - V0
    //       r^2 = du^2 + dv^2
    //       f = K1 * r^2 + K2 * r^4 (radial distortion factor)
    double u = ((this->m_A * x + this->m_B * y + this->m_C) / (this->m_G * x + this->m_H * y + this->m_I));
    double v = ((this->m_D * x + this->m_E * y + this->m_F) / (this->m_G * x + this->m_H * y + this->m_I));
    double du = u - this->m_U0;
    double dv = v - this->m_V0;
    double r2 = du * du + dv * dv;
    double f = this->m_K1 * r2 + this->m_K2 * r2 * r2;
    double U = u + du * f;
    double V = v + dv * f;

    return std::make_pair(U, V);
}

std::pair<double, double> Characteristics::ToSpaceCoordinates(
    const std::pair<double, double>& pixel_coordinates)
{
    double U = pixel_coordinates.first;
    double V = pixel_coordinates.second;
    // U = u + f.du
    // V = u + f.dv
    // with f = K1.r^2 + K2.r^4
    //      r^2 = du^2 + dv^2
    //      du = u - U0
    //      dv = v - V0
    // use U and V as a first estimation of u and v => du and dv
    // calculate u and v, and hope it converges into something after 5 loops
    double u = U;
    double v = V;
    for (int z = 0; z < 5; z++)
    {
        double du = u - this->m_U0;
        double dv = v - this->m_V0;
        double r2 = du * du + dv * dv;
        double ff = this->m_K1 * r2 + this->m_K2 * r2 * r2;

        u = U - du * ff;
        v = V - dv * ff;
    }

    // u = (Ax + By + C) / (Gx + Hy + I)
    // v = (Dx + Ey + F) / (Gx + Hy + I)
    // solve for (x,y):
    // (Gu-A)x + (Hu-B)y = (C-Iu)
    // (Gv-D)x + (Hv-E)y = (F-Iv)
    // ==> Cramer's rule
    // noemer = (Gu-A)(Hv-E) - (Hu-B)(Gv-D)
    // x = ((C-Iu)(Hv-E) - (B-Hu)(Iv-F)) / noemer
    // y = ((A-Gu)(Iv-F) - (C-Iu)(Gv-D)) / noemer
    double noemer = (this->m_G * u - this->m_A) * (this->m_H * v - this->m_E) - (this->m_H * u - this->m_B) * (this->m_G * v - this->m_D);
    assert(noemer != 0.0);
    double x = ((this->m_C - this->m_I * u) * (this->m_H * v - this->m_E) - (this->m_B - this->m_H * u) * (this->m_I * v - this->m_F)) / noemer;
    double y = ((this->m_A - this->m_G * u) * (this->m_I * v - this->m_F) - (this->m_C - this->m_I * u) * (this->m_G * v - this->m_D)) / noemer;
    return std::make_pair(x, y);
}
