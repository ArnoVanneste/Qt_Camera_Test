#include "lmsolver.h"

#include <QDebug>

using namespace Helpers;

OptimizationResult LMSolver::Solve(
    void (*evaluate)(const double *, int, const void *, double *, int *),
    double *parameters,
    int mdata)
{
    lm_control_struct ctrl {
        .ftol = this->m_ftol,
        .xtol = this->m_xtol,
        .gtol = this->m_gtol,
        .epsilon = this->m_epsilon,
        .stepbound = this->m_initial_step_bound,
        .patience = this->m_patience,
        .scale_diag = this->m_scale_diagonal,
        .msgfile = nullptr,
        .verbosity = this->m_verbose_output ? 3 : 0,
        .n_maxpri = -1,
        .m_maxpri = -1
    };

    lm_status_struct stat;

    double *optimized_parameters = (double *)malloc((sizeof(parameters)/sizeof(*parameters)) * sizeof(double));
    PtrDeepCopy<double>(parameters, optimized_parameters, 12);

    lmmin(
        (sizeof(optimized_parameters)/sizeof(*optimized_parameters)),
        optimized_parameters,
        mdata,
        nullptr,
        evaluate,
        &ctrl,
        &stat,
        malloc_array_allocator,
        free_deallocator);

    OptimizationResult result(
        optimized_parameters,
        stat.fnorm,
        stat.nfev,
        (LMSolverStatus)stat.outcome,
        "test",
        stat.userbreak > 0 ? true : false
    );

    // free(optimized_parameters);

    return result;
}
