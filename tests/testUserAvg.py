import time
import pyreclab

if __name__ == '__main__':

   uavg = pyreclab.UserAvg( dataset = 'dataset/u1.base',
                            dlmchar = b'\t',
                            header = False,
                            usercol = 0,
                            itemcol = 1,
                            ratingcol = 2 )

   print( '-> training model' )
   start = time.clock()
   uavg.train()
   end = time.clock()
   print( 'training time: ' + str( end - start ) )

   print( '-> individual test' )
   pred = uavg.predict( '457', '443' )
   print( 'user 457, item 443, prediction ' + str( pred ) )

   ranking = uavg.recommend( '457', 5, includeRated = False )
   print( 'recommendation for user 457: ' + str( ranking ) )

   print( '-> prediction test' )
   start = time.clock()
   predlist, mae, rmse = uavg.test( input_file = 'dataset/u1.test',
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
   recommendList = uavg.testrec( input_file = 'dataset/u1.test',
                                 dlmchar = b'\t',
                                 header = False,
                                 usercol = 0,
                                 itemcol = 1,
                                 ratingcol = 2,
                                 topn = 10,
                                 output_file = 'ranking.json' )
   end = time.clock()
   print( 'recommendation time: ' + str( end - start ) )

