/**
 * @file cuts_numuincl2025.h
 * @brief Header file for definitions of analysis cuts specific to the numu_incl2025
 * analysis.
 * @details This file contains definitions of analysis cuts which can be used
 * to select interactions specific to the numu_incl2025 analysis. The cuts are
 * intended to be used in conjunction with the generic cuts defined in cuts.h.
 * @author mueller@fnal.gov
*/
#ifndef CUTS_NUMUINCL2025_H
#define CUTS_NUMUINCL2025_H
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>

#include "include/utilities.h"
#include "include/variables.h"
#include "include/selectors.h"

/**
 * @namespace cuts::numu_incl2025
 * @brief Namespace for organizing cuts specific to the numu_incl2025 analysis.
 * @details This namespace is intended to be used for organizing cuts which act
 * on interactions specific to the numu_incl2025 analysis. Each cut is implemented as
 * a function which takes an interaction object as an argument and returns a
 * boolean. The function should be templated on the type of interaction object if
 * the cut is intended to be used on both true and reconstructed interactions.
 * @note The namespace is intended to be used in conjunction with the cuts
 * namespace, which is used for organizing generic cuts which act on interactions.
 */
namespace cuts::numu_incl2025
{

    /**
     * @brief Apply a cut on the muon containment.
     * @details The interaction must have a muon which is contained within the
     * detector.
     * @tparam T the type of interaction (true or reco).
     * @param obj the interaction to select on.
     * @return true if the interaction has a muon which is contained within the
     * detector.
     */
    template<class T>
    bool muon_containment_cut(const T & obj, double ke_threshold=25)
    {
        size_t muon_index(selectors::leading_muon(obj, ke_threshold));
        if(muon_index == kNoMatch)
        {
            return false;
        }
        return obj.particles[muon_index].is_contained;
    }
    REGISTER_CUT_SCOPE(RegistrationScope::Both, muon_containment_cut, muon_containment_cut);

    /**
     * @brief Apply a cut on the numu CC that doesn't meet the KE threshold.
     * @details The interaction has a leading muon that doesn't meet the KE threshold.
     * @tparam T the type of interaction (true or reco).
     * @param obj the interaction to select on.
     * @return true if the interaction has a muon that doesn't meet the KE threshold.
     */
    template<class T>
    bool muon_oops_cut(const T & obj, double ke_threshold=25)
    {
        size_t muon_index(selectors::leading_muon(obj, 0));
        if(muon_index == kNoMatch)
        {
            return false;
        }
        if(pvars::ke(obj.particles[muon_index]) < ke_threshold)
        {
            return true;
        }
        return false;
    }
    REGISTER_CUT_SCOPE(RegistrationScope::Both, muon_oops_cut, muon_oops_cut);

}
#endif // CUTS_numu_incl2025_H
