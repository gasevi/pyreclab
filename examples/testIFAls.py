import time
import pyreclab

if __name__ == '__main__':

   ifals = pyreclab.IFAls( dataset = 'dataset/u1.base',
                           dlmchar = b'\t',
                           header = False,
                           usercol = 0,
                           itemcol = 1,
                           observationcol = 2 )

   print( '-> training model' )
   start = time.clock()
   ifals.train( factors = 50, alsNumIter = 5, lambd = 10 )
   end = time.clock()
   print( 'training time: ' + str( end - start ) )

   ranking = ifals.recommend( '457', 5, includeRated = False )
   print( 'recommendation for user 457: ' + str( ranking ) )

   print( '-> recommendation test' )
   start = time.clock()
   recommendList, maprec, ndcg = ifals.testrec( input_file = 'dataset/u1.test',
                                                dlmchar = b'\t',
                                                header = False,
                                                usercol = 0,
                                                itemcol = 1,
                                                ratingcol = 2,
                                                topn = 10,
                                                output_file = 'ranking.json',
                                                relevance_threshold = 2,
                                                includeRated = False )
   end = time.clock()
   print( 'recommendation time: ' + str( end - start ) )

   print( 'MAP: ' + str( maprec ) )
   print( 'nDCG: ' + str( ndcg ) )

   mapUser10 = ifals.MAP( user_id = '10', topn = 10, relevance_threshold = 2, include_rated = False )
   ndcgUser10 = ifals.nDCG( user_id = '10', topn = 10, relevance_threshold = 2, include_rated = False  )
   print( 'user 10 MAP: ' + str( mapUser10 ) )
   print( 'user 10 nDCG: ' + str( ndcgUser10 ) )

