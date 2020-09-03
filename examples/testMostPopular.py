import time
import pyreclab

if __name__ == '__main__':

   model = pyreclab.MostPopular( dataset = 'dataset/u1.base',
                                 dlmchar = b'\t',
                                 header = False,
                                 usercol = 0,
                                 itemcol = 1,
                                 ratingcol = 2 )

   print( '-> training model' )
   start = time.clock()
   model.train( 5, progress = True )
   end = time.clock()
   print( 'training time: ' + str( end - start ) )

   print( '-> individual test' )
   ranking = model.recommend( '457', 5, includeRated = False )
   print( 'user 457, recommended items ' + str( ranking ) )

   print( '-> recommendation test' )
   start = time.clock()
   recommendList, maprec, ndcg = model.testrec( input_file = 'dataset/u1.test',
                                                dlmchar = b'\t',
                                                header = False,
                                                usercol = 0,
                                                itemcol = 1,
                                                ratingcol = 2,
                                                topn = 10,
                                                output_file = 'ranking.json',
                                                includeRated = False )
   end = time.clock()
   print( 'recommendation time: ' + str( end - start ) )

   print( 'MAP: %f' % ( maprec ) )
   print( 'nDCG: %f' % ( ndcg ) )

   precisionUser10 = model.precision( user_id = '10', topn = 10, relevance_threshold = 2, include_rated = False  )
   recallUser10 = model.recall( user_id = '10', topn = 10, relevance_threshold = 2, include_rated = False  )
   aucUser10 = model.AUC( user_id = '10', topn = 10, relevance_threshold = 2, include_rated = False  )
   mapUser10 = model.MAP( user_id = '10', topn = 10, relevance_threshold = 2, include_rated = False )
   ndcgUser10 = model.nDCG( user_id = '10', topn = 10, relevance_threshold = 2, include_rated = False  )
   print( 'user 10 Precision: %f' % ( precisionUser10 ) )
   print( 'user 10 Recall: %f' % ( recallUser10 ) )
   print( 'user 10 AUC: %f' % ( aucUser10 ) )
   print( 'user 10 MAP: %f' % ( mapUser10 ) )
   print( 'user 10 nDCG: %f' % ( ndcgUser10 ) )


