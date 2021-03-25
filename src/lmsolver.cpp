#include "lmsolver.h"

double * LMSolver::optParams = nullptr;

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
    this->lmmin = (void (*)(int n_par, double *par, int m_dat, const void *data,
                      void (*evaluate) (const double *par, int m_dat, const void *data,
                                         double *fvec, int *userbreak),
                       const lm_control_struct *control, lm_status_struct *status,
                       double_array_allocator_t alloc_dp_array,
                       deallocator_t dealloc))QLibrary::resolve("lmmin32.dll", "lmmin");
}

LMSolver::~LMSolver()
{

}

__stdcall double* LMSolver::Allocate(int count) {
    LMSolver::optParams =(double *)malloc(count*sizeof(double));
    return LMSolver::optParams;
}

__stdcall void LMSolver::DeAllocate(void *params) {
    free((double *)params);
}

OptimizationResult LMSolver::Solve(void (*f)(const double* par, int m_dat, const void *data, double *fvec, int *userbreak), double* parameters, int mData) {
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

    OptimizationResult* result = NULL;

    Allocate(sizeof(parameters));
    for(int i = 0; i < (int)sizeof (parameters) - 1; i++) {
        if(parameters[i]) {
            LMSolver::optParams[i] = parameters[i];
        }
    }

    // call native lmmin from lmfit package (modifies optimizedPars)
    this->lmmin(
        sizeof(LMSolver::optParams),
        LMSolver::optParams,
        mData,
        NULL,
        f,
        &ctrl,
        &stat,
        LMSolver::Allocate,
        LMSolver::DeAllocate);

    // extract results from lmmin's result data struct
//    result = new OptimizationResult(
//        optParams,
//        stat.fnorm,
//        stat.nfev,
//        (SolverStatus)stat.outcome,
//        LMSolver.outcomeMessages[stat.outcome],
//        stat.userbreak > 0 ? true : false
//    );

    return OptimizationResult(std::vector<double>(), 0, 0, LMCPP::SolverStatus::ConvergedBoth, "", true);
}

