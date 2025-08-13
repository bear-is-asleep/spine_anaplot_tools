/**
 * @file variables_numuincl2025.h
 * @brief Header file for definitions of analysis variables specific to the
 * numu_incl2025 analysis.
 * @details This file contains definitions of analysis variables which can be
 * used to extract information from interactions specific to the numu_incl2025
 * analysis. Each variable is implemented as a function which takes an
 * interaction object as an argument and returns a double. These are the
 * building blocks for producing high-level plots of the selected interactions.
 * @author mueller@fnal.gov
 */
#ifndef VARS_NUMUINCL2025_H
#define VARS_NUMUINCL2025_H

#include "sbnanaobj/StandardRecord/Proxy/SRProxy.h"
#include "sbnanaobj/StandardRecord/SRInteractionDLP.h"
#include "sbnanaobj/StandardRecord/SRInteractionTruthDLP.h"

#include "include/selectors.h"
#include "include/framework.h"
#include "include/cuts.h"
#include "include/numu_incl2025/cuts_numuincl2025.h"

/**
 * @namespace vars::numu_incl2025
 * @brief Namespace for organizing variables specific to the numu_incl2025 analysis.
 * @details This namespace is intended to be used for organizing variables which
 * act on interactions specific to the numu_incl2025 analysis. Each variable is
 * implemented as a function which takes an interaction object as an argument
 * and returns a double. The function should be templated on the type of
 * interaction object if the variable is intended to be used on both true and
 * reconstructed interactions.
 * @note The namespace is intended to be used in conjunction with the vars
 * namespace, which is used for organizing generic variables which act on
 * interactions.
 */
namespace vars::numu_incl2025
{
    /**
     * @brief Variable for enumerating interaction categories.
     * @details This variable provides a basic categorization of interactions
     * using only signal, neutrino background, and cosmic background as the
     * three categories.
     * 0: 1uX (contained muon and fiducial)
     * 1: 1uX (not contained muon and fiducial)
     * 2: 1uX out of phase space (OOPS)
     * 3: 1uX out of FV (OOFV)
     * 4: Non-AV (out of AV)
     * 5: 1eX (AV nuecc interaction)
     * 6: NC
     * 7: Cosmic
     * 8: Other (shouldn't happen)
     * @tparam T the type of interaction (true or reco).
     * @param obj The interaction to apply the variable on.
     * @param ke_threshold The kinetic energy threshold for the muon. Defaults to 25 MeV.
     * @return the enumerated category of the interaction.
    */
    template<class T>
    double category(const T & obj, double ke_threshold=25)
    {
        bool is_fiducial(cuts::fiducial_cut(obj,{-190.0,190.0,-190.0,190.0,10.0,450.0}));
        bool is_av(cuts::fiducial_cut(obj,{-200.0,200.0,-200.0,200.0,0.0,500.0})); //Just set the bounds to be the active volume
        bool is_neutrino(cuts::neutrino(obj));
        bool is_cc(cuts::iscc(obj));
        bool has_a_muon(cuts::has_muon(obj, {ke_threshold}));
        bool has_electron(cuts::has_electron(obj, {25.0}));

        double cat(8);
        if(cuts::numu_incl2025::muon_containment_cut(obj, ke_threshold) && is_fiducial && is_neutrino && is_cc) cat = 0;
        else if(has_a_muon && is_fiducial && is_neutrino && is_cc) cat = 1;
        else if(cuts::numu_incl2025::muon_oops_cut(obj, ke_threshold) && is_fiducial && is_neutrino && is_cc) cat = 2;
        else if(!is_fiducial && is_av && has_a_muon && is_neutrino && is_cc) cat = 3;
        else if(!is_av && is_neutrino) cat = 4;
        else if(is_av && has_electron && is_neutrino && is_cc) cat = 5;
        else if(is_av && is_neutrino && !is_cc) cat = 6;
        else if(!is_neutrino) cat = 7;

        return cat;
    }
    REGISTER_VAR_SCOPE(RegistrationScope::True, category, category);

}
#endif // VARS_MUON2024_H