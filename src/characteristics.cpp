#include "characteristics.h"

#include <assert.h>
#include <iostream>

static Characteristics* FirstEstimation(std::vector<cv::Point2f> pixelCoordinates, std::vector<cv::Point2f> spaceCoordinates, int width, int height) {


    double X1 = spaceCoordinates[0].x;
    double Y1 = spaceCoordinates[0].y;
    double X2 = spaceCoordinates[1].x;
    double Y2 = spaceCoordinates[1].y;
    double X3 = spaceCoordinates[2].x;
    double Y3 = spaceCoordinates[2].y;
    double X4 = spaceCoordinates[3].x;
    double Y4 = spaceCoordinates[3].y;

    double U1 = pixelCoordinates[0].x;
    double V1 = pixelCoordinates[0].y;
    double U2 = pixelCoordinates[1].x;
    double V2 = pixelCoordinates[1].y;
    double U3 = pixelCoordinates[2].x;
    double V3 = pixelCoordinates[2].y;
    double U4 = pixelCoordinates[3].x;
    double V4 = pixelCoordinates[3].y;

    Characteristics *c = new Characteristics
    (
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
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
    c->I = 1.0;
    // => we end up with 8 linear equations with 8 unknowns
    std::vector<std::vector<double> > a = std::vector<std::vector<double> >(8);
    a[0] = std::vector<double> { X1, Y1, 1.0, 0.0, 0.0, 0.0, -U1 * X1, -U1 * Y1 };
    a[1] = std::vector<double> { 0.0, 0.0, 0.0, X1, Y1, 1.0, -V1 * X1, -V1 * Y1 };
    a[2] = std::vector<double> { X2, Y2, 1.0, 0.0, 0.0, 0.0, -U2 * X2, -U2 * Y2 };
    a[3] = std::vector<double> { 0.0, 0.0, 0.0, X2, Y2, 1.0, -V2 * X2, -V2 * Y2 };
    a[4] = std::vector<double> { X3, Y3, 1.0, 0.0, 0.0, 0.0, -U3 * X3, -U3 * Y3 };
    a[5] = std::vector<double> { 0.0, 0.0, 0.0, X3, Y3, 1.0, -V3 * X3, -V3 * Y3 };
    a[6] = std::vector<double> { X4, Y3, 1.0, 0.0, 0.0, 0.0, -U4 * X4, -U4 * Y4 };
    a[7] = std::vector<double> { 0.0, 0.0, 0.0, X4, Y4, 1.0, -V4 * X4, -V4 * Y4 };
    std::vector<double> b { U1, V1, U2, V2, U3, V3, U4, V4 };
//    Solve(a, b);

    c->A = b[0];
    c->B = b[1];
    c->C = b[2];
    c->D = b[3];
    c->E = b[4];
    c->F = b[5];
    c->G = b[6];
    c->H = b[7];

    return c;

}

QPoint Characteristics::ToPixelCoordinates(QPoint spaceCoordinates)
        {
            double x = spaceCoordinates.x();
            double y = spaceCoordinates.y();
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
            double u = ((A * x + B * y + C) / (G * x + H * y + I));
            double v = ((D * x + E * y + F) / (G * x + H * y + I));
            double du = u - U0;
            double dv = v - V0;
            double r2 = du * du + dv * dv;
            double f = K1 * r2 + K2 * r2 * r2;
            double U = u + du * f;
            double V = v + dv * f;
            return QPoint(U, V);
        }

QPoint Characteristics::ToSpaceCoordinates(QPoint pixelCoordinates)
        {
            double U = pixelCoordinates.x();
            double V = pixelCoordinates.y();
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
                double du = u - U0;
                double dv = v - V0;
                double r2 = du * du + dv * dv;
                double ff = K1 * r2 + K2 * r2 * r2;

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
            double noemer = (G * u - A) * (H * v - E) - (H * u - B) * (G * v - D);
            assert (noemer != 0.0);
            double x = ((C - I * u) * (H * v - E) - (B - H * u) * (I * v - F)) / noemer;
            double y = ((A - G * u) * (I * v - F) - (C - I * u) * (G * v - D)) / noemer;
            return QPoint(x, y);
        }

void Characteristics::Solve(std::vector<std::vector<double> > a, std::vector<double> b)
{
    double aantal = b.size();

    assert (a.size() == aantal);
    for (int row = 0; row < aantal; ++row)
        assert (a[row].size() == aantal);

    assert (a.size() == aantal);
    bool* used = new bool[(int)aantal];
    int x;
    for (x = 0; x < aantal; x++)
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
                std::vector<double> d = std::vector<double>(a[x]);
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
    for (x = 0; x < aantal; x++)
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
