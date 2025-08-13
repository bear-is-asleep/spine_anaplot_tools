import numpy as np

def extract_parameter_name(key,variables):
    """
    Extract the essential parameter name from a systematic key.
    
    Parameters
    ----------
    key : str
        The full systematic key
    variables : list
        The variable names we are evaluating systematic uncertainties for
        
    Returns
    -------
    key : str
        The essential parameter name
    var_name : str
        The variable name we are evaluating systematic uncertainties for
    """
    #If it's a csv name, remove the .csv. We also expect the name to be in the format of {name}_{variable}_cv.csv
    if '.csv' in key:
      key = key.replace('.csv', '')
      keep = key.split('_')[:-1]
      key = '_'.join(keep)

    for var in variables:
      if var in key:
        var_name = var
        break
    
    #First check if it's a stat error
    if 'stat' in key:
      return 'stat',var_name
    
    # Remove the trailing ";1" first
    key = key.replace(';1', '')
    key = key.replace('reco_leading_muon_', '')
    key = key.replace('true_leading_muon_', '')
    key = key.replace('momentum_gev', '')
    key = key.replace('costheta', '')
    key = key.replace('multisigma_', '')
    key = key.replace('multisim_', '')
    key = key.replace('nsigma_', '')
    key = key.replace('GENIEReWeight_SBN_v1_', '')
    #Remove trailing _
    while key[-1] == '_':
      key = key[:-1]
    return key, var_name

def get_avg_fracunc(frac_uncs,cv):
  """
  Get the average fractional uncertainty for a given set of fractional uncertainties
  and the central values. The result is a weighted average of the fractional uncertainties
  
  Parameters
  ----------
  frac_uncs : array-like
    Fractional uncertainties
  cv : array-like
    Central values
    
  Returns
  -------
  avg_frac_unc : float
    Average fractional uncertainty
  """
  assert len(frac_uncs) == len(cv), f"Fractional uncertainties ({len(frac_uncs)}) and central values ({len(cv)}) must have the same length"
  return np.average(frac_uncs,weights=cv)

def convert_smearing_to_response(smearing,eff):
  """
  Convert a smearing matrix to a response matrix by convolving 
  with the efficiency array and normalizing by the truth column

  Parameters
  ----------
  smearing : array-like (N,N)
    The smearing matrix
  eff : array-like (N,)
    The efficiency array

  Returns
  -------
  response : array-like (N,N)
    The response matrix
  """
  denom = np.sum(smearing,axis=0)
  response = np.divide(smearing*eff,denom,out=np.zeros_like(smearing,dtype=float),where=denom!=0)
  return response