#include "optimizationresult.h"

OptimizationResult::OptimizationResult(std::vector<double> optimizedParameters, double errorNorm, int iterations, LMCPP::SolverStatus outcome, std::string message, bool terminatedByUser) {
    this->OptimizedParameters = optimizedParameters;
    this->ErrorNorm = errorNorm;
    this->Iterations = iterations;
    this->Outcome = outcome;
    this->Message = message;
    this->TerminatedByUserRequest = terminatedByUser;
}
