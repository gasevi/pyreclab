import time
import pyreclab

if __name__ == '__main__':

   mpop = pyreclab.MostPopular( dataset = 'dataset/u1.base',
                                dlmchar = b'\t',
                                header = False,
                                usercol = 0,
                                itemcol = 1,
                                ratingcol = 2 )

   print( '-> training model' )
   start = time.clock()
   mpop.train( 5 )
   end = time.clock()
   print( 'training time: ' + str( end - start ) )

   print( '-> individual test' )
   ranking = mpop.recommend( '457', 5, includeRated = False )
   print( 'user 457, recommended items ' + str( ranking ) )

   print( '-> recommendation test' )
   start = time.clock()
   recommendList, maprec, ndcg = mpop.testrec( input_file = 'dataset/u1.test',
                                               dlmchar = b'\t',
                                               header = False,
                                               usercol = 0,
                                               topn = 10,
                                               output_file = 'ranking.json',
                                               includeRated = False )
   end = time.clock()
   print( 'recommendation time: ' + str( end - start ) )

   print( 'MAP: ' + str( maprec ) )
   print( 'nDCG: ' + str( ndcg ) )

   mapUser10 = mpop.MAP( user_id = '10', topn = 10, relevance_threshold = 2, include_rated = False )
   ndcgUser10 = mpop.nDCG( user_id = '10', topn = 10, relevance_threshold = 2, include_rated = False  )
   print( 'user 10 MAP: ' + str( mapUser10 ) )
   print( 'user 10 nDCG: ' + str( ndcgUser10 ) )


