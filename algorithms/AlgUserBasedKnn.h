#ifndef __ALG_USER_BASED_KNN_H__
#define __ALG_USER_BASED_KNN_H__

#include "RecSysAlgorithm.h"

#include <string>


class AlgUserBasedKnn
      : public RecSysAlgorithm
{
public:

   AlgUserBasedKnn( RatingMatrix& ratingMatrix );

   int train();

   int train( size_t k );

   void test( DataFrame& dataFrame );

   double predict( std::string userId, std::string itemId );

private:

   size_t m_knn;

   std::map<std::string, double> m_meanRatingByUser;

   SparseMatrix m_simMatrix;

};

#endif // __ALG_USER_BASED_KNN_H__

