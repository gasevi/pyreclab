import time

import sys
sys.path.append( '/home/gasevi/sandbox/pyreclab/pyreclab/' )

import libpyreclab

if __name__ == '__main__':

   ifals = libpyreclab.IFAls( dataset = 'lastfm-dataset.txt',
                              dlmchar = b'\t',
                              header = False,
                              usercol = 0,
                              itemcol = 1,
                              observationcol = 2 )

   print( '-> training model' )
   start = time.clock()
   ifals.train( factors = 100, maxiter = 2, lamb = 10 )
   end = time.clock()
   print( 'training time: ' + str( end - start ) )

   ranking = ifals.recommend( 'user_000436', 5, includeRated = False )
   print( 'recommendation for user 457: ' + str( ranking ) )

