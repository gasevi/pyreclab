#! /usr/bin/env python

# Append pyreclab temporary directory
import time
import sys
sys.path.append( '.' )
sys.path.append( '..' )
sys.path.append( '../pyreclab/' )

import pyreclab

if __name__ == '__main__':

   mpop = pyreclab.MostPopular( 'dataset/u1.base',
                                dlmchar = b'\t',
                                header = False,
                                usercol = 0,
                                itemcol = 1,
                                ratingcol = 2 )

   print( 'training...' )
   start = time.clock()
   mpop.train( 5 )
   end = time.clock()
   traintime = end - start

   print( 'testing...' )
   start = time.clock()
   predlist = mpop.test( 'dataset/u1.test',
                         dlmchar = b'\t',
                         header = False,
                         usercol = 0,
                         output_file = 'ranking.json' )
   end = time.clock()
   testtime = end - start

   pred = mpop.recommend( '457' )
   print( 'user 457, recommended items ' + str( pred ) )

   print( 'train time: ' + str( traintime ) )
   print( 'test time: ' + str( testtime ) )



