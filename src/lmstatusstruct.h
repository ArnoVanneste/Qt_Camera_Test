#ifndef LMSTATUSSTRUCT_H
#define LMSTATUSSTRUCT_H

namespace LMCPP {
    struct LMStatusStruct
    {
        /// <summary>
        /// norm of the residue vector fvec.
        /// </summary>
        double fnorm;

        /// <summary>
        /// actual number of iterations.
        /// </summary>
        int nfev;

        /// <summary>
        /// Status indicator. Nonnegative values are used as index
        /// for the message text lm_infmsg, set in lmmin.c.
        /// </summary>
        int outcome;

        /// <summary>
        /// Set when function evaluation requests termination.
        /// </summary>
        int userbreak;
    };
}

#endif // LMSTATUSSTRUCT_H
