import time
import pyreclab


def test_funksvd():
    expected_prediction = 4.40228557587
    prediction_epsilon = 0.005
    expected_ranking = ['408', '1449', '513', '603', '480']
    expected_mae = 0.735477664861
    mae_epsilon = 0.00005
    expected_rmse = 0.933232779736
    rmse_epsilon = 0.00005

    svd = pyreclab.SVD(dataset='dataset/u1.base',
                       dlmchar=b'\t',
                       header=False,
                       usercol=0,
                       itemcol=1,
                       ratingcol=2)

    print('-> training model')
    start = time.clock()
    svd.train(factors=1000, maxiter=100, lr=0.01, lamb=0.1)
    end = time.clock()
    print('training time: ' + str(end - start))

    print('-> individual test')
    pred = svd.predict('457', '443')
    print('user 457, item 443, prediction ' + str(pred))
    assert abs(pred-expected_prediction) < prediction_epsilon

    ranking = svd.recommend('457', 5, includeRated=False)
    print('recommendation for user 457: ' + str(ranking))

    assert ranking == expected_ranking

    print('-> prediction test')
    start = time.clock()
    predlist, mae, rmse = svd.test(input_file='dataset/u1.test',
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
    svd.testrec(input_file='dataset/u1.test',
                dlmchar=b'\t',
                header=False,
                usercol=0,
                itemcol=1,
                ratingcol=2,
                topn=10,
                output_file='ranking.json')
    end = time.clock()
    print('recommendation time: ' + str(end - start))
