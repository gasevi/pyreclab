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

    mpop.train(5)

    ranking = mpop.recommend('457', 5, includeRated=False)
    assert ranking == expected_ranking
