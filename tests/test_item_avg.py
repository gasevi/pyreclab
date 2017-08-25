import time
import pyreclab


expected_prediction = 3.7933883667
prediction_epsilon = 0.0000005
expected_ranking = ['1500', '1653', '1599', '1189', '1467']
expected_mae = 0.827568403289
mae_epsilon = 0.00005
expected_rmse = 1.03341137142
rmse_epsilon = 0.00005


def test_item_avg():
    iavg = pyreclab.ItemAvg(dataset='dataset/u1.base',
                            dlmchar=b'\t',
                            header=False,
                            usercol=0,
                            itemcol=1,
                            ratingcol=2)

    print('-> training model')
    start = time.clock()
    iavg.train()
    end = time.clock()
    print('training time: ' + str(end - start))

    print('-> individual test')
    pred = iavg.predict('457', '443')
    print('user 457, item 443, prediction ' + str(pred))
    assert abs(pred-expected_prediction) < prediction_epsilon

    ranking = iavg.recommend('457', 5, includeRated=False)
    print('recommendation for user 457: ' + str(ranking))
    assert ranking == expected_ranking

    print('-> prediction test')
    start = time.clock()
    predlist, mae, rmse = iavg.test(input_file='dataset/u1.test',
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
    iavg.testrec(input_file='dataset/u1.test',
                 dlmchar=b'\t',
                 header=False,
                 usercol=0,
                 itemcol=1,
                 ratingcol=2,
                 topn=10,
                 output_file='ranking.json')
    end = time.clock()
    print('recommendation time: ' + str(end - start))
