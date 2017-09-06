import pyreclab

expected_prediction = 4.40228557587
prediction_epsilon = 0.005
expected_ranking = ['408', '1449', '513', '603', '480']
expected_mae = 0.735477664861
mae_epsilon = 0.00005
expected_rmse = 0.933232779736
rmse_epsilon = 0.00005


def test_funksvd():

    svd = pyreclab.SVD(dataset='dataset/u1.base',
                       dlmchar=b'\t',
                       header=False,
                       usercol=0,
                       itemcol=1,
                       ratingcol=2)

    svd.train(factors=1000, maxiter=100, lr=0.01, lamb=0.1)

    pred = svd.predict('457', '443')
    assert abs(pred-expected_prediction) < prediction_epsilon

    ranking = svd.recommend('457', 5, includeRated=False)

    assert ranking == expected_ranking

    predlist, mae, rmse = svd.test(input_file='dataset/u1.test',
                                   dlmchar=b'\t',
                                   header=False,
                                   usercol=0,
                                   itemcol=1,
                                   ratingcol=2,
                                   output_file='predictions.csv')

    assert abs(mae-expected_mae) < mae_epsilon
    assert abs(rmse-expected_rmse) < rmse_epsilon
