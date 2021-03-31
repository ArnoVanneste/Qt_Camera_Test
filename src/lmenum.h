#ifndef LMENUM_H
#define LMENUM_H

enum LMBackend {
    native_lmmin,
    managed_lmmin
};

enum LMSolverStatus {
    underflow,
    converged_sum_sq,
    converged_param,
    converged_both,
    trapped,
    exhausted,
    failed_ftol,
    failed_xtol,
    failed_gtol,
    crashed,
    exploded,
    stopped
};

#endif // LMENUM_H
