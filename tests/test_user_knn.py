import pyreclab


expected_prediction = 4.33838129044
prediction_epsilon = 0.0000005
expected_ranking = ['1467', '1599', '1642', '1653', '1358']
expected_mae = 0.753399181565
mae_epsilon = 0.00005
expected_rmse = 0.960097945921
rmse_epsilon = 0.00005


def test_user_knn():

    ubknn = pyreclab.UserKnn(dataset='dataset/u1.base',
                             dlmchar=b'\t',
                             header=False,
                             usercol=0,
                             itemcol=1,
                             ratingcol=2)

    ubknn.train(k=100, similarity='pearson')

    pred = ubknn.predict('457', '443')
    assert abs(pred-expected_prediction) < prediction_epsilon

    ranking = ubknn.recommend('457', 5, includeRated=False)
    assert ranking == expected_ranking

    predlist, mae, rmse = ubknn.test(input_file='dataset/u1.test',
                                     dlmchar=b'\t',
                                     header=False,
                                     usercol=0,
                                     itemcol=1,
                                     ratingcol=2,
                                     output_file='predictions.csv')

    assert abs(mae-expected_mae) < mae_epsilon
    assert abs(rmse-expected_rmse) < rmse_epsilon
