import pyreclab

expected_prediction = 4.42406368256
prediction_epsilon = 0.0000005
expected_ranking = ['1367', '1449', '1642', '1594', '850']
expected_mae = 0.747890019338
mae_epsilon = 0.00005
expected_rmse = 0.951344520991
rmse_epsilon = 0.00005


def test_item_knn():
    ibknn = pyreclab.ItemKnn(dataset='dataset/u1.base',
                             dlmchar=b'\t',
                             header=False,
                             usercol=0,
                             itemcol=1,
                             ratingcol=2)

    ibknn.train(k=100, similarity='pearson')

    pred = ibknn.predict('457', '443')
    assert abs(pred-expected_prediction) < prediction_epsilon

    ranking = ibknn.recommend('457', 5, includeRated=False)
    assert ranking == expected_ranking

    predlist, mae, rmse = ibknn.test(input_file='dataset/u1.test',
                                     dlmchar=b'\t',
                                     header=False,
                                     usercol=0,
                                     itemcol=1,
                                     ratingcol=2,
                                     output_file='predictions.csv')

    assert abs(mae-expected_mae) < mae_epsilon
    assert abs(rmse-expected_rmse) < rmse_epsilon
