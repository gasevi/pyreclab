import time
import pyreclab


expected_ranking = ['181', '258', '294', '127', '56']
expected_mae = 0.751129130734
mae_epsilon = 0.00005
expected_rmse = 0.960262431338
rmse_epsilon = 0.00005


def test_most_popular():

    mpop = pyreclab.MostPopular(dataset='dataset/u1.base',
                                dlmchar=b'\t',
                                header=False,
                                usercol=0,
                                itemcol=1,
                                ratingcol=2)

    print('-> training model')
    start = time.clock()
    mpop.train(5)
    end = time.clock()
    print('training time: ' + str(end - start))

    print('-> individual test')
    ranking = mpop.recommend('457', 5, includeRated=False)
    print('user 457, recommended items ' + str(ranking))
    assert ranking == expected_ranking

    print('-> recommendation test')
    start = time.clock()
    mpop.testrec(input_file='dataset/u1.test',
                 dlmchar=b'\t',
                 header=False,
                 usercol=0,
                 topn=10,
                 output_file='ranking.json')
    end = time.clock()
    print('recommendation time: ' + str(end - start))
