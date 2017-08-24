import time
import pyreclab

expected_prediction = 4.026022434234619
prediction_epsilon = 0.005
expected_ranking = ['1682', '1681', '907', '1680', '1679']
expected_mae = 0.850191274015
mae_epsilon = 0.00005
expected_rmse = 1.06299512766
rmse_epsilon = 0.00005


def test_user_avg():
    uavg = pyreclab.UserAvg(dataset='dataset/u1.base',
                            dlmchar=b'\t',
                            header=False,
                            usercol=0,
                            itemcol=1,
                            ratingcol=2)

    print('-> training model')
    start = time.clock()
    uavg.train()
    end = time.clock()
    print('training time: ' + str(end - start))

    print('-> individual test')
    pred = uavg.predict('457', '443')
    print('user 457, item 443, prediction ' + str(pred))
    assert abs(pred-expected_prediction) < prediction_epsilon

    ranking = uavg.recommend('457', 5, includeRated=False)
    print('recommendation for user 457: ' + str(ranking))
    assert ranking == expected_ranking

    print('-> prediction test')
    start = time.clock()
    predlist, mae, rmse = uavg.test(input_file='dataset/u1.test',
                                    dlmchar=b'\t',
                                    header=False,
                                    usercol=0,
                                    itemcol=1,
                                    ratingcol=2,
                                    output_file='predictions.csv')
    end = time.clock()
    print('prediction time: ' + str(end - start))

    print('MAE: ' + str(mae))
    print('RMSE: ' + str(rmse))
    assert abs(mae-expected_mae) < mae_epsilon
    assert abs(rmse-expected_rmse) < rmse_epsilon

    print('-> recommendation test')
    start = time.clock()
    uavg.testrec(input_file='dataset/u1.test',
                 dlmchar=b'\t',
                 header=False,
                 usercol=0,
                 itemcol=1,
                 ratingcol=2,
                 topn=10,
                 output_file='ranking.json')
    end = time.clock()
    print('recommendation time: ' + str(end - start))
