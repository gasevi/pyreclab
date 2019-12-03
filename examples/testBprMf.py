import time
import pyreclab

if __name__ == '__main__':

   bprmf = pyreclab.BprMf( factors = 20,
                           dataset = 'dataset/u1.base',
                           dlmchar = b'\t',
                           header = False,
                           usercol = 0,
                           itemcol = 1,
                           ratingcol = 2 )

   print( '-> training model' )
   start = time.clock()
   bprmf.train( maxiter = 5000000, lr = 0.1, lambda_w = 0.01, lambda_hp = 0.01, lambda_hm = 0.01, progress = True )
   end = time.clock()
   print( 'training time: ' + str( end - start ) )

   ranking = bprmf.recommend( '457', 5, includeRated = False )
   print( 'recommendation for user 457: ' + str( ranking ) )

   print( '-> recommendation test' )
   start = time.clock()
   recommendList, maprec, ndcg = bprmf.testrec( input_file = 'dataset/u1.test',
                                                dlmchar = b'\t',
                                                header = False,
                                                usercol = 0,
                                                itemcol = 1,
                                                ratingcol = 2,
                                                topn = 10,
                                                output_file = 'ranking.json',
                                                relevance_threshold = 0,
                                                includeRated = False )
   end = time.clock()
   print( 'recommendation time: ' + str( end - start ) )

   print( 'MAP: ' + str( maprec ) )
   print( 'nDCG: ' + str( ndcg ) )

   mapUser10 = bprmf.MAP( user_id = '10', topn = 10, relevance_threshold = 2, include_rated = False )
   ndcgUser10 = bprmf.nDCG( user_id = '10', topn = 10, relevance_threshold = 2, include_rated = False  )
   print( 'user 10 MAP: ' + str( mapUser10 ) )
   print( 'user 10 nDCG: ' + str( ndcgUser10 ) )


