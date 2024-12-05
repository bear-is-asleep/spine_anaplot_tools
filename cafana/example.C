/**
 * @file example.C
 * @brief A basic example analysis macro for demonstrating the use of the
 * spine_anaplot_tools/cafana package.
 * @details This macro demonstrates how to use the spine_anaplot_tools/cafana
 * package to perform a basic analysis of a CAFAna file. The macro configures
 * some basic variables and cuts, then runs the analysis over a single sample.
 * @author mueller@fnal.gov
*/
#include "include/variables.h"
#include "include/muon2024/variables_muon2024.h"
#include "include/cuts.h"
#include "include/muon2024/cuts_muon2024.h"
#include "include/preprocessor.h"
#include "include/analysis.h"

#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "sbnana/CAFAna/Core/Tree.h"
#include "sbnana/CAFAna/Core/Cut.h"
#include "sbnana/CAFAna/Core/Spectrum.h"
#include "TDirectory.h"
#include "TFile.h"

void example()
{
    /**
     * @brief Create an instance of the Analysis class.
     * @details This creates an instance of the Analysis class, which is used
     * to run the analysis on the specified samples. The name of the analysis,
     * and therefor the name of the output file, is specified as an argument to
     * the constructor.
     */
    ana::Analysis analysis("/exp/sbnd/data/users/brindenc/xsec_analysis/test/sbruce/example");

    /**
     * @brief Add a sample to the analysis.
     * @details This adds a sample to the analysis by creating a SpectrumLoader
     * object and adding it to the Analysis class. The SpectrumLoader object
     * represents the sample in the analysis, and is used to load the data from
     * the ROOT file and apply the cuts and variables. The name passed to the
     * AddLoader function is used to create a directory in the output ROOT file
     * to store the results of the analysis.
     */
    ana::SpectrumLoader var00("/pnfs/icarus/persistent/users/mueller/spinereco2024/allplanes/detsys_v09_89_01_01/var00_nominal.flat.root");
    analysis.AddLoader("nominal", &var00, true);
    
    /**
     * @brief Add a set of variables for selected interactions to the analysis.
     * @details This adds a set of variables to the analysis by creating a
     * map of variable names and SpillMultiVars that provide the functionality
     * to calculate the variables. These names are used in the TTree that is
     * created by the Tree class to store the results of the analysis.
     */
    std::map<std::string, ana::SpillMultiVar> vars_selected;
    vars_selected.insert({"nu_id", ana::SpillMultiVar(SPINEVAR_RT(vars::neutrino_id, cuts::fiducial_containment_flash_cut_bnb))});
    vars_selected.insert({"baseline", ana::SpillMultiVar(SPINEVAR_RT(vars::true_neutrino_baseline, cuts::fiducial_containment_flash_cut_bnb))});
    vars_selected.insert({"pdg", ana::SpillMultiVar(SPINEVAR_RT(vars::true_neutrino_pdg, cuts::fiducial_containment_flash_cut_bnb))});
    vars_selected.insert({"cc", ana::SpillMultiVar(SPINEVAR_RT(vars::true_neutrino_cc, cuts::fiducial_containment_flash_cut_bnb))});
    vars_selected.insert({"category", ana::SpillMultiVar(SPINEVAR_RT(vars::neutrino_interaction_mode, cuts::fiducial_containment_flash_cut_bnb))});
    vars_selected.insert({"true_edep", ana::SpillMultiVar(SPINEVAR_RT(vars::true_neutrino_energy, cuts::fiducial_containment_flash_cut_bnb))});
    vars_selected.insert({"reco_edep", ana::SpillMultiVar(SPINEVAR_RR(vars::visible_energy, cuts::fiducial_containment_flash_cut_bnb))});
    vars_selected.insert({"flash_time", ana::SpillMultiVar(SPINEVAR_RR(vars::flash_time, cuts::fiducial_containment_flash_cut_bnb))});
    vars_selected.insert({"flash_total", ana::SpillMultiVar(SPINEVAR_RR(vars::flash_total_pe, cuts::fiducial_containment_flash_cut_bnb))});
    vars_selected.insert({"flash_hypothesis", ana::SpillMultiVar(SPINEVAR_RR(vars::flash_hypothesis, cuts::fiducial_containment_flash_cut_bnb))});

    analysis.AddTree("selectedNu", vars_selected, true);

    /**
     * @brief Add a set of variables for signal interactions to the analysis.
     * @details This adds a set of variables to the analysis by creating a
     * map of variable names and SpillMultiVars that provide the functionality
     * to calculate the variables. These names are used in the TTree that is
     * created by the Tree class to store the results of the analysis.
     */
    std::map<std::string, ana::SpillMultiVar> vars_signal;
    vars_signal.insert({"nu_id", ana::SpillMultiVar(SPINEVAR_TT(vars::neutrino_id, cuts::fiducial_containment_neutrino_cut))});
    vars_signal.insert({"category", ana::SpillMultiVar(SPINEVAR_TT(vars::neutrino_interaction_mode, cuts::fiducial_containment_neutrino_cut))});
    analysis.AddTree("signalNu", vars_signal, true);

    /**
     * @brief Run the analysis.
     * @details This runs the analysis on the samples specified by the
     * SpectrumLoaders and variables added to the Analysis class. It loops over
     * each sample (here only one), applies the cuts and variables to the data,
     * and stores the results in a TFile.
     */
    analysis.Go();
}