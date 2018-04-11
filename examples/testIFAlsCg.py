import time
import pyreclab

if __name__ == '__main__':

   ifalscg = pyreclab.IFAlsConjugateGradient( dataset = 'dataset/lastfm-dataset.txt',
                                              dlmchar = b'\t',
                                              header = False,
                                              usercol = 0,
                                              itemcol = 1,
                                              observationcol = 2 )

   print( '-> training model' )
   start = time.clock()
   ifalscg.train( factors = 50, alsNumIter = 5, lambd = 10, cgNumIter = 2 )
   end = time.clock()
   print( 'training time: ' + str( end - start ) )

   ranking = ifalscg.recommend( 'user_000436', 5, includeRated = False )
   print( 'recommendation for user 457: ' + str( ranking ) )

