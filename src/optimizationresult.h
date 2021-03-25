#ifndef OPTIMIZATIONRESULT_H
#define OPTIMIZATIONRESULT_H

#include "solverstatus.h"

#include <iostream>
#include <vector>



class OptimizationResult
{
public:
    OptimizationResult(std::vector<double> optimizedParameters, double errorNorm, int iterations, LMCPP::SolverStatus outcome, std::string message, bool terminatedByUser);

public:
    std::vector<double> OptimizedParameters;
    double ErrorNorm;
    int Iterations;
    LMCPP::SolverStatus Outcome;
    std::string Message;
    bool TerminatedByUserRequest;
};

#endif // OPTIMIZATIONRESULT_H
