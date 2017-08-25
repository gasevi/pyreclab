import time
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

    print('-> training model')
    start = time.clock()
    ibknn.train(k=100, similarity='pearson')
    end = time.clock()
    print('training time: ' + str(end - start))

    print('-> individual test')
    pred = ibknn.predict('457', '443')
    print('user 457, item 443, prediction ' + str(pred))
    assert abs(pred-expected_prediction) < prediction_epsilon

    ranking = ibknn.recommend('457', 5, includeRated=False)
    print('recommendation for user 457: ' + str(ranking))
    assert ranking == expected_ranking

    print('-> prediction test')
    start = time.clock()
    predlist, mae, rmse = ibknn.test(input_file='dataset/u1.test',
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
    ibknn.testrec(input_file='dataset/u1.test',
                  dlmchar=b'\t',
                  header=False,
                  usercol=0,
                  itemcol=1,
                  ratingcol=2,
                  topn=10,
                  output_file='ranking.json')
    end = time.clock()
    print('recommendation time: ' + str(end - start))
