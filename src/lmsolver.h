#ifndef LMSOLVER_H
#define LMSOLVER_H

#include "optimizationresult.h"
#include "characteristics.h"
#include "lmcontrolstruct.h"
#include "LMStatusStruct.h"
#include "lmmin.h"

#include <iostream>
#include <functional>
#include <cstdint>

constexpr double machineEpsilon = 2.2204460492503131e-16; // smallest normalized dp value
constexpr double defaultTolerance = 30.0 * machineEpsilon;

enum LMBackend
{
    NativeLmmin,
    ManagedLmmin
};

class LMSolver
{
public:
    LMSolver();
    ~LMSolver();

    LMSolver(double ftol = defaultTolerance,
            double xtol = defaultTolerance,
            double gtol = defaultTolerance,
            double epsilon = defaultTolerance,
            double stepbound = 100.0,
            int patience = 100,
            bool scaleDiagonal = true,
            bool verbose = false,
            LMBackend optimizerBackend = LMBackend::NativeLmmin);

    double Ftol;
    double Xtol;
    double Gtol;
    double Epsilon;
    double InitialStepbound;
    int Patience;
    bool ScaleDiagonal;
    bool VerboseOutput;
    LMBackend OptimizerBackend;

    OptimizationResult Solve(void (*f)(const double* par, int m_dat, const void *data, double *fvec, int *userbreak), double* parameters, int mData);
    //from lmmin.h
    static __stdcall double* Allocate(int count);
    static __stdcall void DeAllocate(void* params);
    static double *optParams;

private:

};



#endif // LMSOLVER_H
