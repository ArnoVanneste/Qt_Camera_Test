#ifndef LMSOLVER_H
#define LMSOLVER_H

#include <optimizationresult.h>
#include <helpers.h>

#include "lmstruct.h"
#include "lmmin.h"

class LMSolver
{
public:
    LMSolver(
        double ftol = default_tolerance,
        double xtol = default_tolerance,
        double gtol = default_tolerance,
        double epsilon = default_tolerance,
        double initial_step_bound = 100.0,
        int patience = 100.0,
        bool scale_diagonal = true,
        bool verbose_output = false,
        LMBackend optimizer_backend = LMBackend::native_lmmin):
        m_ftol(ftol),
        m_xtol(xtol),
        m_gtol(gtol),
        m_epsilon(epsilon),
        m_initial_step_bound(initial_step_bound),
        m_patience(patience),
        m_scale_diagonal(scale_diagonal),
        m_verbose_output(verbose_output),
        m_optimizer_backend(optimizer_backend)
    {}

    OptimizationResult Solve(
        void (*f)(const double *, int, const void *, double *, int *),
        double *parameters,
        int mdata);

    double m_ftol;
    double m_xtol;
    double m_gtol;
    double m_epsilon;
    double m_initial_step_bound;
    int m_patience;
    bool m_scale_diagonal;
    bool m_verbose_output;
    LMBackend m_optimizer_backend;
private:
    static constexpr double machine_epsilon = 2.2204460492503131e-16;
    static constexpr double default_tolerance = 30 * machine_epsilon;
};

#endif // LMSOLVER_H
