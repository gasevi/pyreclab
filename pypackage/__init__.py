import platform
import sys

if platform.system() == 'Darwin':

   native_version = (2, 7, 10, 'final', 0)
   cur_version = sys.version_info

   if cur_version == native_version:
      from pyreclab.libpyreclab_native_py import MostPopular, UserAvg, ItemAvg, SlopeOne, UserKnn, ItemKnn, SVD, IFAls, IFAlsConjugateGradient, BprMf
   else: # brew version
      from pyreclab.libpyreclab import MostPopular, UserAvg, ItemAvg, SlopeOne, UserKnn, ItemKnn, SVD, IFAls, IFAlsConjugateGradient, BprMf

else:
   from pyreclab.libpyreclab import MostPopular, UserAvg, ItemAvg, SlopeOne, UserKnn, ItemKnn, SVD, IFAls, IFAlsConjugateGradient, BprMf

__all__ = [ 'MostPopular',
            'UserAvg',
            'ItemAvg',
            'SlopeOne',
            'UserKnn',
            'ItemKnn',
            'SVD',
            'IFAls',
            'IFAlsConjugateGradient',
            'BprMf' ]

