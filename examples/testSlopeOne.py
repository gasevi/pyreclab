import time
import pyreclab

if __name__ == '__main__':

   slpone = pyreclab.SlopeOne( dataset = 'dataset/u1.base',
                               dlmchar = b'\t',
                               header = False,
                               usercol = 0,
                               itemcol = 1,
                               ratingcol = 2 )

   print( '-> training model' )
   start = time.clock()
   slpone.train()
   end = time.clock()
   print( 'training time: ' + str( end - start ) )

   print( '-> individual test' )
   pred = slpone.predict( '457', '443' )
   print( 'user 457, item 443, prediction ' + str( pred ) )

   ranking = slpone.recommend( '457', 5, includeRated = False )
   print( 'recommendation for user 457: ' + str( ranking ) )

   print( '-> prediction test' )
   start = time.clock()
   predlist, mae, rmse = slpone.test( input_file = 'dataset/u1.test',
                                      dlmchar = b'\t',
                                      header = False,
                                      usercol = 0,
                                      itemcol = 1,
                                      ratingcol = 2,
                                      output_file = 'predictions.csv' )
   end = time.clock()
   print( 'prediction time: ' + str( end - start ) )

   print( 'MAE: ' + str( mae ) )
   print( 'RMSE: ' + str( rmse ) )

   print( '-> recommendation test' )
   start = time.clock()
   recommendList, maprec, ndcg = slpone.testrec( input_file = 'dataset/u1.test',
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

   mapUser10 = slpone.MAP( user_id = '10', topn = 10, relevance_threshold = 2, include_rated = False )
   ndcgUser10 = slpone.nDCG( user_id = '10', topn = 10, relevance_threshold = 2, include_rated = False  )
   print( 'user 10 MAP: ' + str( mapUser10 ) )
   print( 'user 10 nDCG: ' + str( ndcgUser10 ) )


