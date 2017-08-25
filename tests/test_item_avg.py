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

    iavg.train()

    pred = iavg.predict('457', '443')
    assert abs(pred-expected_prediction) < prediction_epsilon

    ranking = iavg.recommend('457', 5, includeRated=False)
    assert ranking == expected_ranking

    predlist, mae, rmse = iavg.test(input_file='dataset/u1.test',
                                    dlmchar=b'\t',
                                    header=False,
                                    usercol=0,
                                    itemcol=1,
                                    ratingcol=2,
                                    output_file='predictions.csv')

    assert abs(mae-expected_mae) < mae_epsilon
    assert abs(rmse-expected_rmse) < rmse_epsilon
