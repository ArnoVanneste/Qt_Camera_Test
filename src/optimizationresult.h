#ifndef OPTIMIZATIONRESULT_H
#define OPTIMIZATIONRESULT_H

#include <QString>

#include <lmenum.h>

#include <vector>

class OptimizationResult
{
public:
    OptimizationResult(
        double *optimized_parameters,
        double error_norm,
        int iterations,
        LMSolverStatus outcome,
        QString message,
        bool terminated_by_user_request):
        m_error_norm(error_norm),
        m_iterations(iterations),
        m_outcome(outcome),
        m_message(message),
        m_terminated_by_user_request(terminated_by_user_request)
    {
        this->m_optimized_parameters.clear();
        for (unsigned index = 0; index < 12; index++) {
            this->m_optimized_parameters.push_back(optimized_parameters[index]);
        }
    }

    std::vector<double> m_optimized_parameters;
    double m_error_norm;
    int m_iterations;
    LMSolverStatus m_outcome;
    QString m_message;
    bool m_terminated_by_user_request;
private:
};

#endif // OPTIMIZATIONRESULT_H
