import matplotlib.pyplot as plt
import numpy as np

def plot_hist_edges(edges,values,errors,label,ax=None,**pltkwargs):
  """
  Make step plot from edges and values
  
  Parameters
  ----------
  edges : array-like
      Edges of histogram bins
  values : array-like
      Values of histogram bins
  errors : array-like or None
      Errors of histogram bins
  label : str
      Label for legend
  ax : matplotlib.axes.Axes or None
      Axes to plot on. If None, use current axes.
  pltkwargs : dict
      Keyword arguments to pass to matplotlib.pyplot.step()
  
  """
  centers = (edges[1:] + edges[:-1])/2
  if ax is None:
      h = plt.step(edges, list(values)+[values[-1]], where='post', label=label,**pltkwargs)
      if errors is not None: 
          e = plt.errorbar(centers, values, yerr=errors, elinewidth=3,fmt='none', color=h[0].get_color(),alpha=0.6,capsize=7)
      else:
          e = None
  else:
      h = ax.step(edges, list(values)+[values[-1]], where='post', label=label,**pltkwargs)
      if errors is not None: 
          e = ax.errorbar(centers, values, yerr=errors, elinewidth=3,fmt='none', color=h[0].get_color(),alpha=0.6,capsize=7) 
      else:
          e = None
  return h,e



def plot_square_with_bins(matrix, bin_edges, title=None, axis_labels=None,
                               cmap='viridis', figsize=(8, 6)):
    """
    Plot covariance matrix with custom bin sizing based on bin widths.
    
    Parameters:
    -----------
    matrix : 2D numpy array
        Square covariance matrix to plot
    bin_edges : 1D numpy array
        Bin edge values (length should be n_bins + 1)
    title : str, optional
        Title for the plot
    axis_labels : Union[list of str, str], optional
        Labels for the x and y axes. If a single string is provided, it will be used for both axes.
    cmap : str, optional
        Colormap for the plot
    figsize : tuple, optional
        Figure size (width, height)
    
    Returns:
    --------
    fig, ax : matplotlib figure and axis objects
    """
    
    n_bins = matrix.shape[0]
    if matrix.shape[1] != n_bins:
        raise ValueError("Covariance matrix must be square")
    
    if len(bin_edges) != n_bins + 1:
        raise ValueError(f"Bin edges length ({len(bin_edges)}) must be n_bins + 1 ({n_bins + 1})")
    
    # Calculate bin centers
    bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
    
    # Create figure and axis
    fig, ax = plt.subplots(figsize=figsize)
    
    # Create meshgrid for bin centers
    X, Y = np.meshgrid(bin_centers, bin_centers)
    
    # Plot using pcolormesh for more control over bin sizes
    im = ax.pcolormesh(X, Y, matrix, cmap=cmap, shading='auto')
    
    # Add colorbar
    cbar = plt.colorbar(im, ax=ax)
    
    # Set labels and title
    if axis_labels is not None:
        if isinstance(axis_labels, str):
            ax.set_xlabel(axis_labels)
            ax.set_ylabel(axis_labels)
        else:
            ax.set_xlabel(axis_labels[0])
            ax.set_ylabel(axis_labels[1])
    if title is not None:
      ax.set_title(title)
    
    # Set aspect ratio to equal for square bins
    ax.set_aspect('equal')
    
    plt.tight_layout()
    return fig, ax