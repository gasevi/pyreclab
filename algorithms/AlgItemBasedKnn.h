#ifndef __ALG_ITEM_BASED_KNN_H__
#define __ALG_ITEM_BASED_KNN_H__

#include "RecSysAlgorithm.h"

#include <string>


class AlgItemBasedKnn
      : public RecSysAlgorithm
{
public:

   AlgItemBasedKnn( RatingMatrix& ratingMatrix );

   void train();

   void train( size_t k );

   void test( DataFrame& dataFrame );

   double predict( std::string userId, std::string itemId );

private:

   size_t m_knn;

   std::map<std::string, double> m_meanRatingByItem;

   SparseMatrix m_simMatrix;

};

#endif // __ALG_ITEM_BASED_KNN_H__

