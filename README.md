# <center> <span style="color:#00f; font-size: 4em;">*Pyreclab*: Recommendation lab for Python</span> </center>

   *Pyreclab* is a recommendation library designed for training recommendation models with a friendly and easy-to-use interface, keeping a good performance in memory and CPU usage.

   In order to achieve this, *Pyreclab* is built as a Python module to give a friendly access to its algorithms and it is completely developed in C++ to avoid the lack of performace of the interpreted languages.

   At this moment, the following recommendation algorithms are supported:

**Rating Prediction**

  * User Avgerage
  * Item Average
  * Slope One
  * User Based KNN
  * Item Based KNN
  * Funk's SVD

**Item Recommendation**

  * Most Popular


   Although *Pyreclab* can be compiled on most popular operating system, it has been tested on Linux *Ubuntu 16.04*.


## <span style="font-size: 4em;">Build and install</span>

1.- Before start, verify you have *libboost-dev* installed on your system. If not, install it using your favorite package manager.

```sh
$ sudo apt-get install libboost-dev
```

2.- Clone the source code of *Pyreclab* in a local directory.

```sh
$ git clone https://github.com/gasevi/pyreclab.git
```

3.- Build the Python module ( default: *Python 2.7* ).

```sh
$ cd pyreclab
$ cmake .
$ make
```

By default, it will be compiled for *Python 2.7*. If you want to build it for *Python 3*, you can execute the following steps:

```sh
$ cd pyreclab
$ cmake -DPY_MAJOR_VERSION=3 .
$ make
```

4.- Install Pyreclab.

```sh
$ make install
```


## <span style="font-size: 4em;">API Documentation</span>

   Pyreclab provides the following classes for representing each of the recommendation algorithm currenly supported:

 * [pyreclab.UserAvg](#useravg)
 * [pyreclab.ItemAvg](#itemavg)
 * [pyreclab.SlopeOne](#slopeone)
 * [pyreclab.UserKnn](#userknn)
 * [pyreclab.ItemKnn](#itemknn)
 * [pyreclab.SVD](#svd)
 * [pyreclab.MostPopular](#mostpopular)

   So, you can import any of them as follows:

```python
>>> from pyreclab import <RecAlg>
```

   or import the entire module as you prefer

```python
>>> import pyreclab
```

   Afer that, to create an instance of any of these clases, you must provide a dataset file with the training information, which must contain the fields *user_id*, *item_id* and *rating*.

   The following example shows the generic format for creating one of these instances.

```python
>>> obj = pyreclab.RecAlg( dataset = filename,
                           dlmchar = b'\t',
                           header = False,
                           usercol = 0,
                           itemcol = 1,
                           ratingcol = 2 )
```

Where *RecAlg* represents the recommendation algorithm chosen from the previous list, and its parameters are presented in the next table.


| Parameter | Type      | Default value | Description                                                 |
|:----------|:---------:|:-------------:|:------------------------------------------------------------|
| dataset   | mandatory | N.A.          | Dataset filename with fields: userid, itemid and rating     |
| dlmchar   | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header    | optional  | False         | Whether dataset filename contains a header line to skip     |
| usercol   | optional  | 0             | User column position in dataset file                        |
| itemcol   | optional  | 1             | Item column position in dataset file                        |
| rating    | optional  | 2             | Rating column position in dataset file                      |


   Due to the different nature of each algorithm, their train methods can have different parameters. For this reason, they have been described for each class as shown below.


### <a name="useravg"> pyreclab.UserAvg </a>

 * Training

```python
>>> obj.train()
```

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
>>> ranking = obj.recommend( userId, topN )
```

| Parameter | Type      | Default value | Description               |
|:----------|:---------:|:-------------:|:--------------------------|
| userId    | mandatory | N.A.          | User identifier           |
| topN      | optional  | 10            | Top N items to recommend  |

 * Testing and evaluation

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
| rating      | optional  | 2             | Rating column position in dataset file                      |
| output_file | optional  | N.A.          | Output file to write predictions                            |


### <a name="itemavg"> pyreclab.ItemAvg </a>

 * Training

```python
>>> obj.train()
```

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
>>> ranking = obj.recommend( userId, topN )
```

| Parameter | Type      | Default value | Description               |
|:----------|:---------:|:-------------:|:--------------------------|
| userId    | mandatory | N.A.          | User identifier           |
| topN      | optional  | 10            | Top N items to recommend  |

 * Testing and evaluation

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
| rating      | optional  | 2             | Rating column position in dataset file                      |
| output_file | optional  | N.A.          | Output file to write predictions                            |


### <a name="slopeone"> pyreclab.SlopeOne </a>

 * Training

```python
obj.train()
```

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
>>> ranking = obj.recommend( userId, topN )
```

| Parameter | Type      | Default value | Description               |
|:----------|:---------:|:-------------:|:--------------------------|
| userId    | mandatory | N.A.          | User identifier           |
| topN      | optional  | 10            | Top N items to recommend  |

 * Testing and evaluation

```python
predictionList, mae, rmse = obj.test( input_file = testset,
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
| rating      | optional  | 2             | Rating column position in dataset file                      |
| output_file | optional  | N.A.          | Output file to write predictions                            |


### <a name="userknn"> pyreclab.UserKnn </a>

 * Training

```python
>>> obj.train( knn )
```

| Parameter | Type      | Default value | Description         |
|:----------|:---------:|:-------------:|:--------------------|
| knn       | optional  | 10            | K nearest neighbors |

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
>>> ranking = obj.recommend( userId, topN )
```

| Parameter | Type      | Default value | Description               |
|:----------|:---------:|:-------------:|:--------------------------|
| userId    | mandatory | N.A.          | User identifier           |
| topN      | optional  | 10            | Top N items to recommend  |

 * Testing and evaluation

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
| rating      | optional  | 2             | Rating column position in dataset file                      |
| output_file | optional  | N.A.          | Output file to write predictions                            |


### <a name="itemknn"> pyreclab.ItemKnn </a>

 * Training

```python
>>> obj.train( knn )
```

| Parameter | Type      | Default value | Description         |
|:----------|:---------:|:-------------:|:--------------------|
| knn       | optional  | 10            | K nearest neighbors |

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
>>> ranking = obj.recommend( userId, topN )
```

| Parameter | Type      | Default value | Description               |
|:----------|:---------:|:-------------:|:--------------------------|
| userId    | mandatory | N.A.          | User identifier           |
| topN      | optional  | 10            | Top N items to recommend  |

 * Testing and evaluation

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
| rating      | optional  | 2             | Rating column position in dataset file                      |
| output_file | optional  | N.A.          | Output file to write predictions                            |


### <a name="svd"> pyreclab.SVD </a>

 * Training

```python
>>> obj.train( factors = 1000, maxiter = 100, lr = 0.01, lamb = 0.1 )
```

| Parameter   | Type      | Default value | Description                                                 |
|:------------|:---------:|:-------------:|:------------------------------------------------------------|
| factors     | optional  | 1000          | Number of latent factors in matrix factorization            |
| maxiter     | optional  | 100           | Maximum number of iterations reached without convergence    |
| lr          | optional  | 0.01          | Learning rate                                               |
| lamb        | optional  | 0.1           | Regularization parameter                                    |

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
>>> ranking = obj.recommend( userId, topN )
```

| Parameter | Type      | Default value | Description               |
|:----------|:---------:|:-------------:|:--------------------------|
| userId    | mandatory | N.A.          | User identifier           |
| topN      | optional  | 10            | Top N items to recommend  |

 * Testing and evaluation

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
| rating      | optional  | 2             | Rating column position in dataset file                      |
| output_file | optional  | N.A.          | Output file to write predictions                            |



### <a name="mostpopular"> pyreclab.MostPopular </a>

 * Training

```python
>>> obj.train()
```

 * Top-N item recommendation

```python
>>> ranking = obj.recommend( userId, topN )
```

| Parameter | Type      | Default value | Description               |
|:----------|:---------:|:-------------:|:--------------------------|
| userId    | mandatory | N.A.          | User identifier           |
| topN      | optional  | 10            | Top N items to recommend  |


 * Testing

```python
>>> predictionList = obj.test( input_file = testset,
                               dlmchar = b'\t',
                               header = False,
                               usercol = 0,
                               output_file = 'ranking.json',
                               topN = 10 )
```

| Parameter   | Type      | Default value | Description                                                 |
|:------------|:---------:|:-------------:|:------------------------------------------------------------|
| input_file  | mandatory | N.A.          | Testset filename                                            |
| dlmchar     | optional  | tab           | Delimiter character between fields (userid, itemid, rating) |
| header      | optional  | False         | Dataset filename contains first line header to skip         |
| usercol     | optional  | 0             | User column position in dataset file                        |
| output_file | optional  | N.A.          | Output file to write rankings                               |
| topN        | optional  | 10            | Top N items to recommend                                    |


## <span style="font-size: 4em;">On roadmap</span>

 * Add ranking evaluation metrics.
 * Extend support for other operating systems like *Mac OS X* and *Windows*.


