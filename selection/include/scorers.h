/**
 * @file scorers.h
 * @brief Header file for functions which are intended to be used by the user
 * at the configuration file level to define PID / scoring / enumeration of
 * categories.
 * @details This file contains functions which are intended to be used by the
 * user at the configuration file level to define PID / scoring / enumeration
 * of categories. Though these may be implemented as functions in the @ref vars
 * or @ref pvars namespace, they are moved here to provide a cleaner interface.
 * @author mueller@fnal.gov
 */
#ifndef SCORERS_H
#define SCORERS_H

namespace pvars
{
    /**
     * @brief Function pointer for the primary classification function.
     * @details This function pointer is used to allow the user to configure
     * the primary classification function used in the analysis. The primary
     * classification function is used to assign the primary classification of
     * the particle based on the softmax scores of the particle. This method
     * specifically allows the user to override the default primary
     * classification function with a custom one, which can be set in the
     * configuration file.
     */
    extern std::shared_ptr<VarFn<RParticleType>> primfn;

    /**
     * @brief Function pointer for the PID function.
     * @details This function pointer is used to allow the user to configure
     * the PID function used in the analysis. The PID function is used to
     * assign the PID of the particle based on the softmax scores of the
     * particle. This method specifically allows the user to override the
     * default PID function with a custom one, which can be set in the
     * configuration file.
     */
    extern std::shared_ptr<VarFn<RParticleType>> pidfn;
} // namespace pvars
#endif