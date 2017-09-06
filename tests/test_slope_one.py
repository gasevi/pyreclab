import pyreclab


expected_prediction = 4.49626874924
prediction_epsilon = 0.0000005
expected_ranking = ['1500', '1656', '1653', '1651', '1650']
expected_mae = 0.751129130734
mae_epsilon = 0.00005
expected_rmse = 0.960262431338
rmse_epsilon = 0.00005


def test_slope_one():

    slpone = pyreclab.SlopeOne(dataset='dataset/u1.base',
                               dlmchar=b'\t',
                               header=False,
                               usercol=0,
                               itemcol=1,
                               ratingcol=2)

    slpone.train()

    pred = slpone.predict('457', '443')
    assert abs(pred-expected_prediction) < prediction_epsilon

    ranking = slpone.recommend('457', 5, includeRated=False)
    assert ranking == expected_ranking

    predlist, mae, rmse = slpone.test(input_file='dataset/u1.test',
                                      dlmchar=b'\t',
                                      header=False,
                                      usercol=0,
                                      itemcol=1,
                                      ratingcol=2,
                                      output_file='predictions.csv')

    assert abs(mae-expected_mae) < mae_epsilon
    assert abs(rmse-expected_rmse) < rmse_epsilon
