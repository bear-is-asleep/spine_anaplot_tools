import numpy as np
from scipy.stats import binom
import pandas as pd
import matplotlib.pyplot as plt

from artists import SpineArtist
from style import Style
from variable import Variable
from utilities import mark_pot, mark_preliminary

class SpinePurity(SpineArtist):
    """
    A class designed to encapsulate the calculation of the purity
    of the selection. It loads the uncut MC sample and the branches
    contain the cuts in order. A table is then created with the purity
    for each cut, in the order of the cuts list. Also supports plotting
    purity as a function of the number of events in the sample.

    Attributes
    ----------
    _samples : list
        A list of samples to use in the calculation of the purity.
    _categories : dict
        A dictionary mapping the category key to the category name.
    _cuts : dict
        A dictionary mapping the cut key to the cut label.
    _show_option : str
        The option to use when showing the artist.
    _xrange : tuple
        The range of the x-axis. If None, the range is taken from the
        Variable object.
    _xtitle : str
        The title of the x-axis. If None, the title is taken from the
        Variable object.
    _totals : dict
        A dictionary containing the total number of events in each bin
        of the variable.
    """
    def __init__(self, variable, categories, cuts, title,
                 xrange=None, xtitle=None, show_option='table',
                 npts=1e6):
        """
        Parameters
        ----------
        variable : Variable
            The variable to calculate the purity with respect to.
        samples : list
            A list of samples to use in the calculation of the purity.
        categories : dict
            A dictionary mapping the category key to the category name.
        cuts : dict
            A dictionary mapping the cut key to the cut label.
        title : str
            The title of the artist.
        xrange : tuple, optional
            The range of the x-axis. If None, the range is taken from
            the Variable object. The default is None.
        xtitle : str, optional
            The title of the x-axis. If None, the title is taken from
            the Variable object. The default is None.
        show_option : str, optional
            The option to use when showing the artist. The default is
            'table.'
        """
        super().__init__(title)
        self._variable = variable
        self._samples = list()
        self._categories = categories
        self._cuts = cuts
        self._title = title
        self._xrange = xrange
        self._xtitle = xtitle
        self._show_option = show_option
        self._totals = dict()
    def draw(self, ax, show_option, percentage=True, show_seqpur=True,
             show_unseqpur=True, yrange=None, style=None,
             logx=False, logy=False):
        """
        Draw the artist on the given axis.

        Parameters
        ----------
        ax : matplotlib.axes.Axes
            The axis to draw the artist on.
        show_option : str
            The option to use when showing the artist.
        percentage : bool, optional
            A flag to indicate if the purity should be displayed
            as a percentage. The default is True.
        show_seqpur : bool, optional
            A flag to indicate if the sequential (cumulative)
            purity should be shown. The default is True.
        show_unseqpur : bool, optional
            A flag to indicate if the unsequential (single-cut)
            purity should be shown. The default is True.
        yrange : tuple, optional
            The range of the y-axis. The default is None.
        style : str, optional
            The style to use when drawing the artist. The default is
            None.
        logx : bool, optional
            A flag to indicate if the x-axis should be displayed on
            a log scale. The default is False.
        logy : bool, optional
            A flag to indicate if the y-axis should be displayed on
            a log scale. The default is False.
        """
        ax.set_title(self._title)

        # Get a list of the groups, while respecting the order of the
        # groups as they are configured in the analysis block. It is
        # possible that duplicates exist (multiple categories within
        # the same group), so care must be taken to ensure that the
        # groups are unique AND in the correct order.
        groups = list()
        for category in self._categories.values():
            if category not in groups:
                groups.append(category)

        if show_option == 'table':
            # Lambda formatter to round the values to two decimal
            # places, display as percentage (if toggled), and add super
            # and subscripts for the error values.
            if percentage:
                formatter = lambda x: rf'${100*x:.2f}$'
                diff_key = 'Differential\nPurity [%]'
                cumu_key = 'Cumulative\nPurity [%]'
            else:
                formatter = lambda x: rf'${x:.2f}$'
                diff_key = 'Differential\nPurity'
                cumu_key = 'Cumulative\nPurity'

            # Clear up the axis because we are going to draw a table
            # on it (no need for any other plot elements).
            ax.axis('off')

            # Create the table data.
            results = pd.DataFrame({r'   ': list(), r'Cut': list(), r'Purity [%]': list(), r'Cumulative [%]': list()})
            group_endpoint = dict()

            # Loop over the groups requested in the plot.
            for group in groups:
                # Extract the cv, msigma, and psigma values for the
                # group.
                _, cv, msigma, psigma = self.reduce(group, significance=0.6827)