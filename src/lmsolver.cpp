#include "lmsolver.h"

LMSolver::LMSolver(double ftol,
                double xtol,
                double gtol,
                double epsilon,
                double stepbound,
                int patience,
                bool scaleDiagonal,
                bool verbose,
                LMBackend optimizerBackend) {

    this->Ftol = ftol;
    this->Xtol = xtol;
    this->Gtol = gtol;
    this->Epsilon = epsilon;
    this->InitialStepbound = stepbound;
    this->Patience = patience;
    this->ScaleDiagonal = scaleDiagonal;
    this->VerboseOutput = verbose;
    this->OptimizerBackend = optimizerBackend;
}

LMSolver::~LMSolver()
{

}

OptimizationResult* LMSolver::Solve(void (*f)(const double* par, int m_dat, const void *data, double *fvec, int *userbreak), double* parameters, int mData) {
    lm_control_struct ctrl = {
        .ftol = this->Ftol,
        .xtol = this->Xtol,
        .gtol = this->Gtol,
        .epsilon = this->Epsilon,
        .stepbound = this->InitialStepbound,
        .patience = this->Patience,
        .scale_diag = this->ScaleDiagonal ? 1 : 0,
        .msgfile = NULL,
        .verbosity = this->VerboseOutput ? 3 : 0,
        .n_maxpri = -1,
        .m_maxpri = -1
    };

    lm_status_struct stat;

    OptimizationResult* result;

    this->optParams = (double *)malloc(sizeof(parameters)*sizeof(double));
    for(int i = 0; i < (int)sizeof (parameters) - 1; i++) {
        if(parameters[i]) {
            this->optParams[i] = parameters[i];
        }
    }

    // call native lmmin from lmfit package (modifies optimizedPars)
    lmmin(
        sizeof(this->optParams),
        this->optParams,
        mData,
        NULL,
        f,
        &ctrl,
        &stat,
        malloc_array_allocator,
        free_deallocator);

    // extract results from lmmin's result data struct
    result = new OptimizationResult(this->optParams, stat.fnorm, stat.nfev, (LMCPP::SolverStatus)stat.outcome, "Outcome message", stat.userbreak > 0 ? true : false);

    return result;
}

