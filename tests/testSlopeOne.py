#! /usr/bin/env python

# Append pyreclab temporary directory
import time
import sys
sys.path.append( '.' )
sys.path.append( '..' )
sys.path.append( '../pyreclab/' )

import pyreclab

if __name__ == '__main__':

   slpone = pyreclab.SlopeOne( dataset = 'dataset/u1.base',
                               dlmchar = b'\t',
                               header = False,
                               usercol = 0,
                               itemcol = 1,
                               ratingcol = 2 )

   print( 'training...' )
   start = time.clock()
   slpone.train()
   end = time.clock()
   traintime = end - start

   print( 'testing...' )
   start = time.clock()
   predlist, mae, rmse = slpone.test( input_file = 'dataset/u1.test',
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

   print( 'train time: ' + str( traintime ) )
   print( 'test time: ' + str( testtime ) )

   pred = slpone.predict( '457', '443' )
   print( 'user 457, item 443, prediction ' + str( pred ) )

   ranking = slpone.recommend( '457', 5 )
   print( 'recommendation for user 457: ' + str( ranking ) )


