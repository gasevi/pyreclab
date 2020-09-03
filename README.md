# <center> <span style="color:#00f; font-size: 4em;">*pyRecLab*: Recommendation lab for Python</span> </center>

## <span style="font-size: 4em;">Overview</span>

   *pyRecLab* is a recommendation library designed for training recommendation models with a friendly and easy-to-use interface, keeping a good performance in memory and CPU usage.

   In order to achieve this, *pyRecLab* is built as a Python module to give a friendly access to its algorithms and it is completely developed in C++ to avoid the lack of performace of the interpreted languages.

   At this moment, the following recommendation algorithms are supported:

<!-- Tick      : &#10003 -->
<!-- Bold tick : &#10004 -->

| RecSys Algorithm             | Rating Prediction | Item Recommendation | Implicit Feedback |
|:-----------------------------|:-----------------:|:-------------------:|:-----------------:|
| User Average                 | x                 | x                   |                   |
| Item Average                 | x                 | x                   |                   |
| Slope One                    | x                 | x                   |                   |
| User Based KNN               | x                 | x                   |                   |
| Item Based KNN               | x                 | x                   |                   |
| Funk's SVD                   | x                 | x                   |                   |
| Most Popular                 |                   | x                   |                   |
| ALS                          |                   | x                   | x                 |
| ALS with Conjugate Gradient  |                   | x                   | x                 |
| BPR for Matrix Factorization |                   | x                   | x                 |


<!--
**Rating Prediction**

  * User Average
  * Item Average
  * Slope One
  * User Based KNN
  * Item Based KNN
  * Funk's SVD

**Item Recommendation**

  * Most Popular

**Implicit Feedback**

  * ALS
  * ALS with Conjugate Gradient method
-->

   Although *pyRecLab* can be compiled on most popular operating system, it has been tested on the following distributions.

| Operating System | Version              |
|:-----------------|:---------------------|
| Ubuntu           | 16.04                |
| CentOS           | 6.4                  |
| Mac OS X         | 10.11 ( El Capitan ) |
| Mac OS X         | 10.12 ( Sierra )     |


## <span style="font-size: 4em;">Citations</span>

If you use this library, please cite:

@inproceedings{1706.06291v2,
   author        = {Gabriel Sepulveda and Vicente Dominguez and Denis Parra},
   title         = {pyRecLab: A Software Library for Quick Prototyping of Recommender Systems},
   year          = {2017},
   month         = {August},
   eprint        = {arXiv:1706.06291v2},
   keywords      = {Recommender Systems, Software Development, Recommender Library, Python Library}
}

[Check out our paper](https://arxiv.org/pdf/1706.06291v2.pdf)


## <span style="font-size: 4em;">Install</span>

*pyRecLab* can be installed directly using pip as follow:

### Python 2

```sh
pip install pyreclab
```

### Python 3

```sh
pip3 install pyreclab
```


## <span style="font-size: 4em;">Build and Install</span>

In case there is not a package available for your favorite operating system, you can build *pyRecLab* through the following steps:

1.- Before starting, verify you have *libpython-dev*, *boost* and *cmake* installed on your system. If not, install it through your distribution's package manager, as shown next.

### Debian based OS's ( Ubuntu )

```sh
$ sudo apt-get install libpython-dev
$ sudo apt-get install cmake
$ sudo apt-get install libboost-dev
```

Note: for Python 3.x, install *libpython3-dev* instead of *libpython-dev*.

### RedHat based OS's ( CentOS )

```sh
$ yum install python-devel
$ yum install cmake
$ yum install boost-devel
```

### MAC OS X

```sh
$ brew install cmake
$ brew install boost
```

2.- Clone the source code of *pyRecLab* in a local directory.

```sh
$ git clone https://github.com/gasevi/pyreclab.git
```

3.- Build the Python module ( default: *Python 2.7* ).

```sh
$ cd pyreclab
$ cmake .
$ make
```

By default, pyRecLab will be compiled for *Python 2.7*. If you want to build it for *Python 3.x*, you can execute the following steps:

```sh
$ cd pyreclab
$ cmake -DCMAKE_PYTHON_VERSION=3.x .
$ make
```

4.- Install pyRecLab.

```sh
$ sudo make install
```


## <span style="font-size: 4em;">API Documentation</span>

   pyRecLab provides the following classes for representing each of the recommendation algorithm currenly supported:

 * [pyreclab.UserAvg](#useravg)
 * [pyreclab.ItemAvg](#itemavg)
 * [pyreclab.SlopeOne](#slopeone)
 * [pyreclab.UserKnn](#userknn)
 * [pyreclab.ItemKnn](#itemknn)
 * [pyreclab.SVD](#svd)
 * [pyreclab.MostPopular](#mostpopular)
 * [pyreclab.IFAls](#ifals)
 * [pyreclab.IFAlsConjugateGradient](#ifalscg)
 * [pyreclab.BprMf](#bprmf)

   So, you can import any of them as follows:

```python
>>> from pyreclab import <RecAlg>
```

   or import the entire module as you prefer

```python
>>> import pyreclab
```

   Due to the different nature of each algorithm, their implementations can have some variations on its parameters. For this reason, each class is described in detail in the following sections.


### <a name="useravg"> pyreclab.UserAvg </a>

 * Instance creation

```python
>>> obj = pyreclab.UserAvg( dataset = filename,
                            dlmchar = b'\t',
                            header = False,
                            usercol = 0,
                            itemcol = 1,
                            ratingcol = 2 )
```

| Parameter | Type      | Default value | Description                                                 |
|:----------|:---------:|:-------------:|:------------------------------------------------------------|
| dataset   | mandatory | N.A.          | Dataset filename with fields: userid, itemid and rating     |
| dlmchar   | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header    | optional  | False         | Whether dataset filename contains a header line to skip     |
| usercol   | optional  | 0             | User column position in dataset file                        |
| itemcol   | optional  | 1             | Item column position in dataset file                        |
| ratingcol | optional  | 2             | Rating column position in dataset file                      |

 * Training

```python
>>> obj.train( progress = False )
```

| Parameter | Type      | Default value | Description       |
|:----------|:---------:|:-------------:|:------------------|
| progress  | optional  | False         | Show progress bar |

 * Rating prediction

```python
>>> prediction = obj.predict( userId, itemId )
```

| Parameter | Type      | Default value | Description       |
|:----------|:---------:|:-------------:|:------------------|
| userId    | mandatory | N.A.          | User identifier   |
| itemId    | mandatory | N.A.          | Item identifier   |

 * Top-N item recommendation

```python
>>> ranking = obj.recommend( userId, topN, includeRated )
```

| Parameter    | Type      | Default value | Description                               |
|:-------------|:---------:|:-------------:|:------------------------------------------|
| userId       | mandatory | N.A.          | User identifier                           |
| topN         | optional  | 10            | Top N items to recommend                  |
| includeRated | optional  | False         | Include rated items in ranking generation |

 * Testing and evaluation for prediction

```python
>>> predictionList, mae, rmse = obj.test( input_file = testset,
                                          dlmchar = b'\t',
                                          header = False,
                                          usercol = 0,
                                          itemcol = 1,
                                          ratingcol = 2,
                                          output_file = 'predictions.csv' )
```

| Parameter   | Type      | Default value | Description                                                 |
|:------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file  | mandatory | N.A.          | Testset filename                                            |
| dlmchar     | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header      | optional  | False         | Dataset filename contains first line header to skip         |
| usercol     | optional  | 0             | User column position in dataset file                        |
| itemcol     | optional  | 1             | Item column position in dataset file                        |
| ratingcol   | optional  | 2             | Rating column position in dataset file                      |
| output_file | optional  | N.A.          | Output file to write predictions                            |


 * Testing for recommendation

```python
>>> recommendationList, map, ndcg = obj.testrec( input_file = testset,
                                                 dlmchar = b'\t',
                                                 header = False,
                                                 usercol = 0,
                                                 itemcol = 1,
                                                 ratingcol = 2,
                                                 topn = 10,
                                                 output_file = 'ranking.json',
                                                 relevance_threshold = 0,
                                                 includeRated = False )
```

| Parameter    | Type      | Default value | Description                                                 |
|:-------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file   | mandatory | N.A.          | Testset filename                                            |
| dlmchar      | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header       | optional  | False         | Dataset filename contains first line header to skip         |
| usercol      | optional  | 0             | User column position in dataset file                        |
| itemcol      | optional  | 1             | Item column position in dataset file                        |
| ratingcol    | optional  | 2             | Rating column position in dataset file                      |
| topn         | optional  | 10            | Top N items to recommend                                    |
| output_file  | optional  | N.A.          | Output file to write predictions                            |
| relevance_threshold | optional  | 0          | Lower threshold to consider an item as relevant ( threshold value included ) |
| includeRated | optional  | False         | Include rated items in ranking generation                   |


 * Precision

```python
>>> precision = obj.precision( user_id,
                               retrieved,
                               topn = 10,
                               relevance_threshold = 0,
                               include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Recall

```python
>>> recall = obj.recall( user_id,
                         retrieved,
                         topn = 10,
                         relevance_threshold = 0,
                         include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Area Under the ROC Curve (AUC)

```python
>>> auc = obj.AUC( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Mean Average precision

```python
>>> map = obj.MAP( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Normalized Discounted Cumulative Gain

```python
>>> map = obj.nDCG( user_id,
                    retrieved,
                    topn = 10,
                    relevance_threshold = 0,
                    include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


### <a name="itemavg"> pyreclab.ItemAvg </a>

 * Instance creation

```python
>>> obj = pyreclab.ItemAvg( dataset = filename,
                            dlmchar = b'\t',
                            header = False,
                            usercol = 0,
                            itemcol = 1,
                            ratingcol = 2 )
```

| Parameter | Type      | Default value | Description                                                 |
|:----------|:---------:|:-------------:|:------------------------------------------------------------|
| dataset   | mandatory | N.A.          | Dataset filename with fields: userid, itemid and rating     |
| dlmchar   | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header    | optional  | False         | Whether dataset filename contains a header line to skip     |
| usercol   | optional  | 0             | User column position in dataset file                        |
| itemcol   | optional  | 1             | Item column position in dataset file                        |
| ratingcol | optional  | 2             | Rating column position in dataset file                      |

 * Training

```python
>>> obj.train( progress = False )
```

| Parameter | Type      | Default value | Description       |
|:----------|:---------:|:-------------:|:------------------|
| progress  | optional  | False         | Show progress bar |

 * Rating prediction

```python
>>> prediction = obj.predict( userId, itemId )
```

| Parameter | Type      | Default value | Description       |
|:----------|:---------:|:-------------:|:------------------|
| userId    | mandatory | N.A.          | User identifier   |
| itemId    | mandatory | N.A.          | Item identifier   |

 * Top-N item recommendation

```python
>>> ranking = obj.recommend( userId, topN, includeRated )
```

| Parameter    | Type      | Default value | Description                               |
|:-------------|:---------:|:-------------:|:------------------------------------------|
| userId       | mandatory | N.A.          | User identifier                           |
| topN         | optional  | 10            | Top N items to recommend                  |
| includeRated | optional  | False         | Include rated items in ranking generation |

 * Testing and evaluation for prediction

```python
>>> predictionList, mae, rmse = obj.test( input_file = testset,
                                          dlmchar = b'\t',
                                          header = False,
                                          usercol = 0,
                                          itemcol = 1,
                                          ratingcol = 2,
                                          output_file = 'predictions.csv' )
```

| Parameter   | Type      | Default value | Description                                                 |
|:------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file  | mandatory | N.A.          | Testset filename                                            |
| dlmchar     | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header      | optional  | False         | Dataset filename contains first line header to skip         |
| usercol     | optional  | 0             | User column position in dataset file                        |
| itemcol     | optional  | 1             | Item column position in dataset file                        |
| ratingcol   | optional  | 2             | Rating column position in dataset file                      |
| output_file | optional  | N.A.          | Output file to write predictions                            |

 * Testing for recommendation

```python
>>> recommendationList, map, ndcg = obj.testrec( input_file = testset,
                                                 dlmchar = b'\t',
                                                 header = False,
                                                 usercol = 0,
                                                 itemcol = 1,
                                                 ratingcol = 2,
                                                 topn = 10,
                                                 output_file = 'ranking.json',
                                                 relevance_threshold = 2,
                                                 includeRated = False )
```

| Parameter    | Type      | Default value | Description                                                 |
|:-------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file   | mandatory | N.A.          | Testset filename                                            |
| dlmchar      | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header       | optional  | False         | Dataset filename contains first line header to skip         |
| usercol      | optional  | 0             | User column position in dataset file                        |
| itemcol      | optional  | 1             | Item column position in dataset file                        |
| ratingcol    | optional  | 2             | Rating column position in dataset file                      |
| topn         | optional  | 10            | Top N items to recommend                                    |
| output_file  | optional  | N.A.          | Output file to write predictions                            |
| relevance_threshold | optional  | 0          | Lower threshold to consider an item as relevant ( threshold value included ) |
| includeRated | optional  | False         | Include rated items in ranking generation                   |


 * Precision

```python
>>> precision = obj.precision( user_id,
                               retrieved,
                               topn = 10,
                               relevance_threshold = 0,
                               include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Recall

```python
>>> recall = obj.recall( user_id,
                         retrieved,
                         topn = 10,
                         relevance_threshold = 0,
                         include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Area Under the ROC Curve (AUC)

```python
>>> auc = obj.AUC( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Mean Average precision

```python
>>> map = obj.MAP( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Normalized Discounted Cumulative Gain

```python
>>> map = obj.nDCG( user_id,
                    retrieved,
                    topn = 10,
                    relevance_threshold = 0,
                    include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


### <a name="slopeone"> pyreclab.SlopeOne </a>

 * Instance creation

```python
>>> obj = pyreclab.SlopeOne( dataset = filename,
                             dlmchar = b'\t',
                             header = False,
                             usercol = 0,
                             itemcol = 1,
                             ratingcol = 2 )
```

| Parameter | Type      | Default value | Description                                                 |
|:----------|:---------:|:-------------:|:------------------------------------------------------------|
| dataset   | mandatory | N.A.          | Dataset filename with fields: userid, itemid and rating     |
| dlmchar   | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header    | optional  | False         | Whether dataset filename contains a header line to skip     |
| usercol   | optional  | 0             | User column position in dataset file                        |
| itemcol   | optional  | 1             | Item column position in dataset file                        |
| ratingcol | optional  | 2             | Rating column position in dataset file                      |

 * Training

```python
>>> obj.train( progress = False )
```

| Parameter | Type      | Default value | Description       |
|:----------|:---------:|:-------------:|:------------------|
| progress  | optional  | False         | Show progress bar |

 * Rating prediction

```python
prediction = obj.predict( userId, itemId )
```

| Parameter | Type      | Default value | Description       |
|:----------|:---------:|:-------------:|:------------------|
| userId    | mandatory | N.A.          | User identifier   |
| itemId    | mandatory | N.A.          | Item identifier   |

 * Top-N item recommendation

```python
>>> ranking = obj.recommend( userId, topN, includeRated )
```

| Parameter    | Type      | Default value | Description                               |
|:-------------|:---------:|:-------------:|:------------------------------------------|
| userId       | mandatory | N.A.          | User identifier                           |
| topN         | optional  | 10            | Top N items to recommend                  |
| includeRated | optional  | False         | Include rated items in ranking generation |

 * Testing and evaluation for prediction

```python
>>> predictionList, mae, rmse = obj.test( input_file = testset,
                                          dlmchar = b'\t',
                                          header = False,
                                          usercol = 0,
                                          itemcol = 1,
                                          ratingcol = 2,
                                          output_file = 'predictions.csv' )
```

| Parameter   | Type      | Default value | Description                                                 |
|:------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file  | mandatory | N.A.          | Testset filename                                            |
| dlmchar     | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header      | optional  | False         | Dataset filename contains first line header to skip         |
| usercol     | optional  | 0             | User column position in dataset file                        |
| itemcol     | optional  | 1             | Item column position in dataset file                        |
| ratingcol   | optional  | 2             | Rating column position in dataset file                      |
| output_file | optional  | N.A.          | Output file to write predictions                            |

 * Testing for recommendation

```python
>>> recommendationList, map, ndcg = obj.testrec( input_file = testset,
                                                 dlmchar = b'\t',
                                                 header = False,
                                                 usercol = 0,
                                                 itemcol = 1,
                                                 ratingcol = 2,
                                                 topn = 10,
                                                 output_file = 'ranking.json',
                                                 relevance_threshold = 2,
                                                 includeRated = False )
```

| Parameter    | Type      | Default value | Description                                                 |
|:-------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file   | mandatory | N.A.          | Testset filename                                            |
| dlmchar      | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header       | optional  | False         | Dataset filename contains first line header to skip         |
| usercol      | optional  | 0             | User column position in dataset file                        |
| itemcol      | optional  | 1             | Item column position in dataset file                        |
| ratingcol    | optional  | 2             | Rating column position in dataset file                      |
| topn         | optional  | 10            | Top N items to recommend                                    |
| output_file  | optional  | N.A.          | Output file to write predictions                            |
| relevance_threshold | optional  | 0          | Lower threshold to consider an item as relevant ( threshold value included ) |
| includeRated | optional  | False         | Include rated items in ranking generation                   |


 * Precision

```python
>>> precision = obj.precision( user_id,
                               retrieved,
                               topn = 10,
                               relevance_threshold = 0,
                               include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Recall

```python
>>> recall = obj.recall( user_id,
                         retrieved,
                         topn = 10,
                         relevance_threshold = 0,
                         include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Area Under the ROC Curve (AUC)

```python
>>> auc = obj.AUC( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Mean Average precision

```python
>>> map = obj.MAP( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Normalized Discounted Cumulative Gain

```python
>>> map = obj.nDCG( user_id,
                    retrieved,
                    topn = 10,
                    relevance_threshold = 0,
                    include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


### <a name="userknn"> pyreclab.UserKnn </a>

 * Instance creation

```python
>>> obj = pyreclab.UserKnn( dataset = filename,
                            dlmchar = b'\t',
                            header = False,
                            usercol = 0,
                            itemcol = 1,
                            ratingcol = 2 )
```

| Parameter | Type      | Default value | Description                                                 |
|:----------|:---------:|:-------------:|:------------------------------------------------------------|
| dataset   | mandatory | N.A.          | Dataset filename with fields: userid, itemid and rating     |
| dlmchar   | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header    | optional  | False         | Whether dataset filename contains a header line to skip     |
| usercol   | optional  | 0             | User column position in dataset file                        |
| itemcol   | optional  | 1             | Item column position in dataset file                        |
| ratingcol | optional  | 2             | Rating column position in dataset file                      |

 * Training

```python
>>> obj.train( knn, similarity, progress = False )
```

| Parameter  | Type      | Default value | Valid values      | Description         |
|:-----------|:---------:|:-------------:|:-----------------:|:--------------------|
| knn        | optional  | 10            | positive integer  | K nearest neighbors |
| similarity | optional  | 'pearson'     |'pearson', 'cosine'| Similarity metric   |
| progress   | optional  | False         |                   | Show progress bar   |

 * Rating prediction

```python
>>> prediction = obj.predict( userId, itemId )
```

| Parameter | Type      | Default value | Description       |
|:----------|:---------:|:-------------:|:------------------|
| userId    | mandatory | N.A.          | User identifier   |
| itemId    | mandatory | N.A.          | Item identifier   |

 * Top-N item recommendation

```python
>>> ranking = obj.recommend( userId, topN, includeRated )
```

| Parameter    | Type      | Default value | Description                               |
|:-------------|:---------:|:-------------:|:------------------------------------------|
| userId       | mandatory | N.A.          | User identifier                           |
| topN         | optional  | 10            | Top N items to recommend                  |
| includeRated | optional  | False         | Include rated items in ranking generation |

 * Testing and evaluation for prediction

```python
>>> predictionList, mae, rmse = obj.test( input_file = testset,
                                          dlmchar = b'\t',
                                          header = False,
                                          usercol = 0,
                                          itemcol = 1,
                                          ratingcol = 2,
                                          output_file = 'predictions.csv' )
```

| Parameter   | Type      | Default value | Description                                                 |
|:------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file  | mandatory | N.A.          | Testset filename                                            |
| dlmchar     | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header      | optional  | False         | Dataset filename contains first line header to skip         |
| usercol     | optional  | 0             | User column position in dataset file                        |
| itemcol     | optional  | 1             | Item column position in dataset file                        |
| ratingcol   | optional  | 2             | Rating column position in dataset file                      |
| output_file | optional  | N.A.          | Output file to write predictions                            |

 * Testing for recommendation

```python
>>> recommendationList, map, ndcg = obj.testrec( input_file = testset,
                                                 dlmchar = b'\t',
                                                 header = False,
                                                 usercol = 0,
                                                 itemcol = 1,
                                                 ratingcol = 2,
                                                 topn = 10,
                                                 output_file = 'ranking.json',
                                                 relevance_threshold = 2,
                                                 includeRated = False )
```

| Parameter    | Type      | Default value | Description                                                 |
|:-------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file   | mandatory | N.A.          | Testset filename                                            |
| dlmchar      | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header       | optional  | False         | Dataset filename contains first line header to skip         |
| usercol      | optional  | 0             | User column position in dataset file                        |
| itemcol      | optional  | 1             | Item column position in dataset file                        |
| ratingcol    | optional  | 2             | Rating column position in dataset file                      |
| topn         | optional  | 10            | Top N items to recommend                                    |
| output_file  | optional  | N.A.          | Output file to write predictions                            |
| relevance_threshold | optional  | 0          | Lower threshold to consider an item as relevant ( threshold value included ) |
| includeRated | optional  | False         | Include rated items in ranking generation                   |


 * Precision

```python
>>> precision = obj.precision( user_id,
                               retrieved,
                               topn = 10,
                               relevance_threshold = 0,
                               include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Recall

```python
>>> recall = obj.recall( user_id,
                         retrieved,
                         topn = 10,
                         relevance_threshold = 0,
                         include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Area Under the ROC Curve (AUC)

```python
>>> auc = obj.AUC( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Mean Average precision

```python
>>> map = obj.MAP( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Normalized Discounted Cumulative Gain

```python
>>> map = obj.nDCG( user_id,
                    retrieved,
                    topn = 10,
                    relevance_threshold = 0,
                    include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


### <a name="itemknn"> pyreclab.ItemKnn </a>

 * Instance creation

```python
>>> obj = pyreclab.ItemKnn( dataset = filename,
                            dlmchar = b'\t',
                            header = False,
                            usercol = 0,
                            itemcol = 1,
                            ratingcol = 2 )
```

| Parameter | Type      | Default value | Description                                                 |
|:----------|:---------:|:-------------:|:------------------------------------------------------------|
| dataset   | mandatory | N.A.          | Dataset filename with fields: userid, itemid and rating     |
| dlmchar   | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header    | optional  | False         | Whether dataset filename contains a header line to skip     |
| usercol   | optional  | 0             | User column position in dataset file                        |
| itemcol   | optional  | 1             | Item column position in dataset file                        |
| ratingcol | optional  | 2             | Rating column position in dataset file                      |

 * Training

```python
>>> obj.train( knn, similarity, progress = False )
```

| Parameter  | Type      | Default value | Valid values      | Description         |
|:-----------|:---------:|:-------------:|:-----------------:|:--------------------|
| knn        | optional  | 10            | positive integer  | K nearest neighbors |
| similarity | optional  | 'pearson'     |'pearson', 'cosine'| Similarity metric   |
| progress   | optional  | False         |                   | Show progress bar   |

 * Rating prediction

```python
>>> prediction = obj.predict( userId, itemId )
```

| Parameter | Type      | Default value | Description       |
|:----------|:---------:|:-------------:|:------------------|
| userId    | mandatory | N.A.          | User identifier   |
| itemId    | mandatory | N.A.          | Item identifier   |

 * Top-N item recommendation

```python
>>> ranking = obj.recommend( userId, topN, includeRated )
```

| Parameter    | Type      | Default value | Description                               |
|:-------------|:---------:|:-------------:|:------------------------------------------|
| userId       | mandatory | N.A.          | User identifier                           |
| topN         | optional  | 10            | Top N items to recommend                  |
| includeRated | optional  | False         | Include rated items in ranking generation |

 * Testing and evaluation for prediction

```python
>>> predictionList, mae, rmse = obj.test( input_file = testset,
                                          dlmchar = b'\t',
                                          header = False,
                                          usercol = 0,
                                          itemcol = 1,
                                          ratingcol = 2,
                                          output_file = 'predictions.csv' )
```

| Parameter   | Type      | Default value | Description                                                 |
|:------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file  | mandatory | N.A.          | Testset filename                                            |
| dlmchar     | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header      | optional  | False         | Dataset filename contains first line header to skip         |
| usercol     | optional  | 0             | User column position in dataset file                        |
| itemcol     | optional  | 1             | Item column position in dataset file                        |
| ratingcol   | optional  | 2             | Rating column position in dataset file                      |
| output_file | optional  | N.A.          | Output file to write predictions                            |

 * Testing for recommendation

```python
>>> recommendationList, map, ndcg = obj.testrec( input_file = testset,
                                                 dlmchar = b'\t',
                                                 header = False,
                                                 usercol = 0,
                                                 itemcol = 1,
                                                 ratingcol = 2,
                                                 topn = 10,
                                                 output_file = 'ranking.json',
                                                 relevance_threshold = 2,
                                                 includeRated = False )
```

| Parameter    | Type      | Default value | Description                                                 |
|:-------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file   | mandatory | N.A.          | Testset filename                                            |
| dlmchar      | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header       | optional  | False         | Dataset filename contains first line header to skip         |
| usercol      | optional  | 0             | User column position in dataset file                        |
| itemcol      | optional  | 1             | Item column position in dataset file                        |
| ratingcol    | optional  | 2             | Rating column position in dataset file                      |
| topn         | optional  | 10            | Top N items to recommend                                    |
| output_file  | optional  | N.A.          | Output file to write predictions                            |
| relevance_threshold | optional  | 0          | Lower threshold to consider an item as relevant ( threshold value included ) |
| includeRated | optional  | False         | Include rated items in ranking generation                   |


 * Precision

```python
>>> precision = obj.precision( user_id,
                               retrieved,
                               topn = 10,
                               relevance_threshold = 0,
                               include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Recall

```python
>>> recall = obj.recall( user_id,
                         retrieved,
                         topn = 10,
                         relevance_threshold = 0,
                         include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Area Under the ROC Curve (AUC)

```python
>>> auc = obj.AUC( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Mean Average precision

```python
>>> map = obj.MAP( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Normalized Discounted Cumulative Gain

```python
>>> map = obj.nDCG( user_id,
                    retrieved,
                    topn = 10,
                    relevance_threshold = 0,
                    include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


### <a name="svd"> pyreclab.SVD </a>

 * Instance creation

```python
>>> obj = pyreclab.SVD( factors = 1000,
                        dataset = filename,
                        dlmchar = b'\t',
                        header = False,
                        usercol = 0,
                        itemcol = 1,
                        ratingcol = 2 )
```

| Parameter | Type      | Default value | Description                                                 |
|:----------|:---------:|:-------------:|:------------------------------------------------------------|
| factors   | optional  | 1000          | Number of latent factors in matrix factorization            |
| dataset   | mandatory | N.A.          | Dataset filename with fields: userid, itemid and rating     |
| dlmchar   | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header    | optional  | False         | Whether dataset filename contains a header line to skip     |
| usercol   | optional  | 0             | User column position in dataset file                        |
| itemcol   | optional  | 1             | Item column position in dataset file                        |
| ratingcol | optional  | 2             | Rating column position in dataset file                      |

 * Training

```python
>>> obj.train( maxiter = 100, lr = 0.01, lamb = 0.1, progress = False )
```

| Parameter | Type      | Default value | Description                                                 |
|:----------|:---------:|:-------------:|:------------------------------------------------------------|
| maxiter   | optional  | 100           | Maximum number of iterations reached without convergence    |
| lr        | optional  | 0.01          | Learning rate                                               |
| lamb      | optional  | 0.1           | Regularization parameter                                    |
| progress  | optional  | False         | Show progress bar                                           |

 * Rating prediction

```python
>>> prediction = obj.predict( userId, itemId )
```

| Parameter | Type      | Default value | Description       |
|:----------|:---------:|:-------------:|:------------------|
| userId    | mandatory | N.A.          | User identifier   |
| itemId    | mandatory | N.A.          | Item identifier   |

 * Top-N item recommendation

```python
>>> ranking = obj.recommend( userId, topN, includeRated )
```

| Parameter    | Type      | Default value | Description                               |
|:-------------|:---------:|:-------------:|:------------------------------------------|
| userId       | mandatory | N.A.          | User identifier                           |
| topN         | optional  | 10            | Top N items to recommend                  |
| includeRated | optional  | False         | Include rated items in ranking generation |

 * Testing and evaluation for prediction

```python
>>> predictionList, mae, rmse = obj.test( input_file = testset,
                                          dlmchar = b'\t',
                                          header = False,
                                          usercol = 0,
                                          itemcol = 1,
                                          ratingcol = 2,
                                          output_file = 'predictions.csv' )
```

| Parameter   | Type      | Default value | Description                                                 |
|:------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file  | mandatory | N.A.          | Testset filename                                            |
| dlmchar     | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header      | optional  | False         | Dataset filename contains first line header to skip         |
| usercol     | optional  | 0             | User column position in dataset file                        |
| itemcol     | optional  | 1             | Item column position in dataset file                        |
| ratingcol   | optional  | 2             | Rating column position in dataset file                      |
| output_file | optional  | N.A.          | Output file to write predictions                            |

 * Testing for recommendation

```python
>>> recommendationList, map, ndcg = obj.testrec( input_file = testset,
                                                 dlmchar = b'\t',
                                                 header = False,
                                                 usercol = 0,
                                                 itemcol = 1,
                                                 ratingcol = 2,
                                                 topn = 10,
                                                 output_file = 'ranking.json',
                                                 relevance_threshold = 2,
                                                 includeRated = False )
```

| Parameter    | Type      | Default value | Description                                                 |
|:-------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file   | mandatory | N.A.          | Testset filename                                            |
| dlmchar      | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header       | optional  | False         | Dataset filename contains first line header to skip         |
| usercol      | optional  | 0             | User column position in dataset file                        |
| itemcol      | optional  | 1             | Item column position in dataset file                        |
| ratingcol    | optional  | 2             | Rating column position in dataset file                      |
| topn         | optional  | 10            | Top N items to recommend                                    |
| output_file  | optional  | N.A.          | Output file to write predictions                            |
| relevance_threshold | optional  | 0          | Lower threshold to consider an item as relevant ( threshold value included ) |
| includeRated | optional  | False         | Include rated items in ranking generation                   |


 * Precision

```python
>>> precision = obj.precision( user_id,
                               retrieved,
                               topn = 10,
                               relevance_threshold = 0,
                               include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Recall

```python
>>> recall = obj.recall( user_id,
                         retrieved,
                         topn = 10,
                         relevance_threshold = 0,
                         include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Area Under the ROC Curve (AUC)

```python
>>> auc = obj.AUC( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Mean Average precision

```python
>>> map = obj.MAP( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Normalized Discounted Cumulative Gain

```python
>>> map = obj.nDCG( user_id,
                    retrieved,
                    topn = 10,
                    relevance_threshold = 0,
                    include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Loss

```python
>>> current_loss = obj.loss()
```


 * Reset factors

```python
>>> obj.reset()
```


### <a name="mostpopular"> pyreclab.MostPopular </a>

 * Instance creation

```python
>>> obj = pyreclab.MostPopular( dataset = filename,
                                dlmchar = b'\t',
                                header = False,
                                usercol = 0,
                                itemcol = 1,
                                ratingcol = 2 )
```

| Parameter | Type      | Default value | Description                                                 |
|:----------|:---------:|:-------------:|:------------------------------------------------------------|
| dataset   | mandatory | N.A.          | Dataset filename with fields: userid, itemid and rating     |
| dlmchar   | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header    | optional  | False         | Whether dataset filename contains a header line to skip     |
| usercol   | optional  | 0             | User column position in dataset file                        |
| itemcol   | optional  | 1             | Item column position in dataset file                        |
| ratingcol | optional  | 2             | Rating column position in dataset file                      |

 * Training

```python
>>> obj.train( progress = False )
```

| Parameter | Type      | Default value | Description       |
|:----------|:---------:|:-------------:|:------------------|
| progress  | optional  | False         | Show progress bar |

 * Top-N item recommendation

```python
>>> ranking = obj.recommend( userId, topN, includeRated )
```

| Parameter    | Type      | Default value | Description                               |
|:-------------|:---------:|:-------------:|:------------------------------------------|
| userId       | mandatory | N.A.          | User identifier                           |
| topN         | optional  | 10            | Top N items to recommend                  |
| includeRated | optional  | False         | Include rated items in ranking generation |


 * Testing for recommendation

```python
>>> recommendationList, map, ndcg = obj.testrec( input_file = testset,
                                                 dlmchar = b'\t',
                                                 header = False,
                                                 usercol = 0,
                                                 itemcol = 1,
                                                 ratingcol = 2,
                                                 topn = 10,
                                                 output_file = 'ranking.json',
                                                 relevance_threshold = 2,
                                                 includeRated = False )
```

| Parameter    | Type      | Default value | Description                                                 |
|:-------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file   | mandatory | N.A.          | Testset filename                                            |
| dlmchar      | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header       | optional  | False         | Dataset filename contains first line header to skip         |
| usercol      | optional  | 0             | User column position in dataset file                        |
| itemcol      | optional  | 1             | Item column position in dataset file                        |
| ratingcol    | optional  | 2             | Rating column position in dataset file                      |
| output_file  | optional  | N.A.          | Output file to write rankings                               |
| topN         | optional  | 10            | Top N items to recommend                                    |
| relevance_threshold | optional  | 0          | Lower threshold to consider an item as relevant ( threshold value included ) |
| includeRated | optional  | False         | Include rated items in ranking generation                   |


 * Precision

```python
>>> precision = obj.precision( user_id,
                               retrieved,
                               topn = 10,
                               relevance_threshold = 0,
                               include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Recall

```python
>>> recall = obj.recall( user_id,
                         retrieved,
                         topn = 10,
                         relevance_threshold = 0,
                         include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Area Under the ROC Curve (AUC)

```python
>>> auc = obj.AUC( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Mean Average precision

```python
>>> map = obj.MAP( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Normalized Discounted Cumulative Gain

```python
>>> map = obj.nDCG( user_id,
                    retrieved,
                    topn = 10,
                    relevance_threshold = 0,
                    include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


### <a name="ifals"> pyreclab.IFAls </a>

 * Instance creation

```python
>>> obj = pyreclab.IFAls( factors = 50,
                          dataset = filename,
                          dlmchar = b'\t',
                          header = False,
                          usercol = 0,
                          itemcol = 1,
                          observationcol = 2 )
```

| Parameter      | Type      | Default value | Description                                                 |
|:---------------|:---------:|:-------------:|:------------------------------------------------------------|
| factors        | optional  | 50            | Number of latent factors in matrix factorization            |
| dataset        | mandatory | N.A.          | Dataset filename with fields: userid, itemid and rating     |
| dlmchar        | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header         | optional  | False         | Whether dataset filename contains a header line to skip     |
| usercol        | optional  | 0             | User column position in dataset file                        |
| itemcol        | optional  | 1             | Item column position in dataset file                        |
| observationcol | optional  | 2             | Observation column position in dataset file                      |

 * Training

```python
>>> obj.train( maxiter, lambd, progress = False )
```

| Parameter   | Type      | Default value | Description                                                 |
|:------------|:---------:|:-------------:|:------------------------------------------------------------|
| alsNumIter  | optional  | 5             | Number of iterations in ALS algorithm                       |
| lambd       | optional  | 10            | Regularization parameter                                    |
| progress    | optional  | False         | Show progress bar                                           |

 * Top-N item recommendation

```python
>>> ranking = obj.recommend( userId, topN, includeRated )
```

| Parameter    | Type      | Default value | Description                               |
|:-------------|:---------:|:-------------:|:------------------------------------------|
| userId       | mandatory | N.A.          | User identifier                           |
| topN         | optional  | 10            | Top N items to recommend                  |
| includeRated | optional  | False         | Include rated items in ranking generation |


 * Testing for recommendation

```python
>>> recommendationList, map, ndcg = obj.testrec( input_file = testset,
                                                 dlmchar = b'\t',
                                                 header = False,
                                                 usercol = 0,
                                                 itemcol = 1,
                                                 ratingcol = 2,
                                                 topn = 10,
                                                 output_file = 'ranking.json',
                                                 relevance_threshold = 2,
                                                 includeRated = False )
```

| Parameter    | Type      | Default value | Description                                                 |
|:-------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file   | mandatory | N.A.          | Testset filename                                            |
| dlmchar      | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header       | optional  | False         | Dataset filename contains first line header to skip         |
| usercol      | optional  | 0             | User column position in dataset file                        |
| itemcol      | optional  | 1             | Item column position in dataset file                        |
| ratingcol    | optional  | 2             | Rating column position in dataset file                      |
| output_file  | optional  | N.A.          | Output file to write rankings                               |
| topN         | optional  | 10            | Top N items to recommend                                    |
| relevance_threshold | optional  | 0          | Lower threshold to consider an item as relevant ( threshold value included ) |
| includeRated | optional  | False         | Include rated items in ranking generation                   |


 * Precision

```python
>>> precision = obj.precision( user_id,
                               retrieved,
                               topn = 10,
                               relevance_threshold = 0,
                               include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Recall

```python
>>> recall = obj.recall( user_id,
                         retrieved,
                         topn = 10,
                         relevance_threshold = 0,
                         include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Area Under the ROC Curve (AUC)

```python
>>> auc = obj.AUC( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Mean Average precision

```python
>>> map = obj.MAP( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Normalized Discounted Cumulative Gain

```python
>>> map = obj.nDCG( user_id,
                    retrieved,
                    topn = 10,
                    relevance_threshold = 0,
                    include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Reset factors

```python
>>> obj.reset()
```


### <a name="ifalscg"> pyreclab.IFAlsConjugateGradient </a>

 * Instance creation

```python
>>> obj = pyreclab.IFAlsConjugateGradient( factors = 50,
                                           dataset = filename,
                                           dlmchar = b'\t',
                                           header = False,
                                           usercol = 0,
                                           itemcol = 1,
                                           observationcol = 2 )
```

| Parameter      | Type      | Default value | Description                                                 |
|:---------------|:---------:|:-------------:|:------------------------------------------------------------|
| factors        | optional  | 50            | Number of latent factors in matrix factorization            |
| dataset        | mandatory | N.A.          | Dataset filename with fields: userid, itemid and rating     |
| dlmchar        | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header         | optional  | False         | Whether dataset filename contains a header line to skip     |
| usercol        | optional  | 0             | User column position in dataset file                        |
| itemcol        | optional  | 1             | Item column position in dataset file                        |
| observationcol | optional  | 2             | Observation column position in dataset file                 |

 * Training

```python
>>> obj.train( alsNumIter, lambd, cgNumIter, progress = False )
```

| Parameter   | Type      | Default value | Description                                                 |
|:------------|:---------:|:-------------:|:------------------------------------------------------------|
| alsNumIter  | optional  | 5             | Number of iterations in ALS algorithm                       |
| lambd       | optional  | 10            | Regularization parameter                                    |
| cgNumIter   | optional  | 2             | Number of iterations in Conjugate Gradient algorithm        |
| progress    | optional  | False         | Show progress bar                                           |

 * Top-N item recommendation

```python
>>> ranking = obj.recommend( userId, topN, includeRated )
```

| Parameter    | Type      | Default value | Description                               |
|:-------------|:---------:|:-------------:|:------------------------------------------|
| userId       | mandatory | N.A.          | User identifier                           |
| topN         | optional  | 10            | Top N items to recommend                  |
| includeRated | optional  | False         | Include rated items in ranking generation |


 * Testing for recommendation

```python
>>> recommendationList, map, ndcg = obj.testrec( input_file = testset,
                                                 dlmchar = b'\t',
                                                 header = False,
                                                 usercol = 0,
                                                 itemcol = 1,
                                                 ratingcol = 2,
                                                 topn = 10,
                                                 output_file = 'ranking.json',
                                                 relevance_threshold = 2,
                                                 includeRated = False )
```

| Parameter    | Type      | Default value | Description                                                 |
|:-------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file   | mandatory | N.A.          | Testset filename                                            |
| dlmchar      | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header       | optional  | False         | Dataset filename contains first line header to skip         |
| usercol      | optional  | 0             | User column position in dataset file                        |
| itemcol      | optional  | 1             | Item column position in dataset file                        |
| ratingcol    | optional  | 2             | Rating column position in dataset file                      |
| output_file  | optional  | N.A.          | Output file to write rankings                               |
| topN         | optional  | 10            | Top N items to recommend                                    |
| relevance_threshold | optional  | 0          | Lower threshold to consider an item as relevant ( threshold value included ) |
| includeRated | optional  | False         | Include rated items in ranking generation                   |


 * Precision

```python
>>> precision = obj.precision( user_id,
                               retrieved,
                               topn = 10,
                               relevance_threshold = 0,
                               include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Recall

```python
>>> recall = obj.recall( user_id,
                         retrieved,
                         topn = 10,
                         relevance_threshold = 0,
                         include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Area Under the ROC Curve (AUC)

```python
>>> auc = obj.AUC( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Mean Average precision

```python
>>> map = obj.MAP( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Normalized Discounted Cumulative Gain

```python
>>> map = obj.nDCG( user_id,
                    retrieved,
                    topn = 10,
                    relevance_threshold = 0,
                    include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Reset factors

```python
>>> obj.reset()
```


### <a name="bprmf"> pyreclab.BprMf </a>

 * Instance creation

```python
>>> obj = pyreclab.BprMf( factors = 20,
                          dataset = filename,
                          dlmchar = b'\t',
                          header = False,
                          usercol = 0,
                          itemcol = 1,
                          observationcol = 2 )
```

| Parameter      | Type      | Default value | Description                                                 |
|:---------------|:---------:|:-------------:|:------------------------------------------------------------|
| factors        | optional  | 20            | Number of latent factors in matrix factorization            |
| dataset        | mandatory | N.A.          | Dataset filename with fields: userid, itemid and rating     |
| dlmchar        | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header         | optional  | False         | Whether dataset filename contains a header line to skip     |
| usercol        | optional  | 0             | User column position in dataset file                        |
| itemcol        | optional  | 1             | Item column position in dataset file                        |
| observationcol | optional  | 2             | Observation column position in dataset file                 |

 * Training

```python
>>> obj.train( maxiter = 100,
               lr = 0.1,
               lambda_w = 0.01,
               lambda_hp = 0.01,
               lambda_hm = 0.01,
               progress = True )
```

| Parameter   | Type      | Default value | Description                                                         |
|:------------|:---------:|:-------------:|:--------------------------------------------------------------------|
| maxiter     | optional  | 100           | Number of iterations                                                |
| lr          | optional  | 0.1           | Learning rate                                                       |
| lambda_w    | optional  | 0.01          | Regularization parameter for the user features                      |
| lambda_hp   | optional  | 0.01          | Regularization parameter for the item features and positive updates |
| lambda_hm   | optional  | 0.01          | Regularization parameter for the item features and negative updates |
| progress    | optional  | False         | Show progress bar                                                   |

 * Top-N item recommendation

```python
>>> ranking = obj.recommend( userId, topN, includeRated )
```

| Parameter    | Type      | Default value | Description                               |
|:-------------|:---------:|:-------------:|:------------------------------------------|
| userId       | mandatory | N.A.          | User identifier                           |
| topN         | optional  | 10            | Top N items to recommend                  |
| includeRated | optional  | False         | Include rated items in ranking generation |


 * Testing for recommendation

```python
>>> recommendationList, map, ndcg = obj.testrec( input_file = testset,
                                                 dlmchar = b'\t',
                                                 header = False,
                                                 usercol = 0,
                                                 itemcol = 1,
                                                 ratingcol = 2,
                                                 topn = 10,
                                                 output_file = 'ranking.json',
                                                 relevance_threshold = 2,
                                                 includeRated = False )
```

| Parameter    | Type      | Default value | Description                                                 |
|:-------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file   | mandatory | N.A.          | Testset filename                                            |
| dlmchar      | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header       | optional  | False         | Dataset filename contains first line header to skip         |
| usercol      | optional  | 0             | User column position in dataset file                        |
| itemcol      | optional  | 1             | Item column position in dataset file                        |
| ratingcol    | optional  | 2             | Rating column position in dataset file                      |
| output_file  | optional  | N.A.          | Output file to write rankings                               |
| topN         | optional  | 10            | Top N items to recommend                                    |
| relevance_threshold | optional  | 0          | Lower threshold to consider an item as relevant ( threshold value included ) |
| includeRated | optional  | False         | Include rated items in ranking generation                   |


 * Precision

```python
>>> precision = obj.precision( user_id,
                               retrieved,
                               topn = 10,
                               relevance_threshold = 0,
                               include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Recall

```python
>>> recall = obj.recall( user_id,
                         retrieved,
                         topn = 10,
                         relevance_threshold = 0,
                         include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Area Under the ROC Curve (AUC)

```python
>>> auc = obj.AUC( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Mean Average precision

```python
>>> map = obj.MAP( user_id,
                   retrieved,
                   topn = 10,
                   relevance_threshold = 0,
                   include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |


 * Normalized Discounted Cumulative Gain

```python
>>> map = obj.nDCG( user_id,
                    retrieved,
                    topn = 10,
                    relevance_threshold = 0,
                    include_rated = False )
```

| Parameter           | Type      | Default value | Description                                                 |
|:--------------------|:---------:|:-------------:|:------------------------------------------------------------|
| user_id             | mandatory | N.A.          | User identifier                                             |
| retrieved           | optional  | N.A.          | Recommendation list for user 'user_id'                      |
| topn                | optional  | 10            | Top N items to recommend. If 'retrieved' is provided, this value will be set to 'retrieved' length |
| relevance_threshold | optional  | 0             | Lower threshold to consider an item as relevant ( threshold value included ) |
| include_rated       | optional  | False         | Include rated items in ranking generation                   |

 * Loss

```python
>>> current_loss = obj.loss()
```

 * Reset factors

```python
>>> obj.reset()
```


## <span style="font-size: 4em;">On roadmap</span>

 * Add *Windows* support.
 * Multi-threading.

