#! /usr/bin/env python

# Append pylibrec temporary directory
import time
import sys
sys.path.append( '.' )
sys.path.append( '..' )
sys.path.append( '../pylibrec/' )

import pylibrec

if __name__ == '__main__':

   svd = pylibrec.SVD( 'dataset/u1.base',
                       dlmchar = b'\t',
                       header = False,
                       usercol = 0,
                       itemcol = 1,
                       ratingcol = 2 )

   print( 'training...' )
   start = time.clock()
   svd.train( factors = 1000, maxiter = 100, lr = 0.01, lamb = 0.1 )
   end = time.clock()
   traintime = end - start

   print( 'testing...' )
   start = time.clock()
   predlist, mae, rmse = svd.test( 'dataset/u1.test',
                                   dlmchar = b'\t',
                                   header = False,
                                   usercol = 0,
                                   itemcol = 1,
                                   ratingcol = 2,
                                   output_file = 'predictions.csv' )
   end = time.clock()
   testtime = end - start

   print( 'MAE: ' + str( mae ) )
   print( 'RMSE: ' + str( rmse ) )

   pred = svd.predict( '457', '443' )
   print( 'user 457, item 443, prediction ' + str( pred ) )

   print( 'train time: ' + str( traintime ) )
   print( 'test time: ' + str( testtime ) )



