#ifndef __ALG_FUNK_SVD_H__
#define __ALG_FUNK_SVD_H__

#include "RecSysAlgorithm.h"

#include <string>


class AlgFunkSvd
      : public RecSysAlgorithm
{
public:

   AlgFunkSvd( RatingMatrix& ratingMatrix );

   ~AlgFunkSvd();

   int train();

   int train( size_t factors, size_t maxiter, float lrate, float lambda );

   void test( DataFrame& dataFrame );

   double predict( std::string userId, std::string itemId );

private:

   void reset( size_t factors, size_t maxiter, float lrate, float lambda );

   double predict( size_t userrow, size_t itemcol );

   bool converged( double loss );

   void adaptLR( double iter );

   double innerProduct( double v1[], double v2[], size_t size );

   std::map<std::string, double> m_meanRatingByUser;

   size_t m_nfactors;

   double** m_userP;

   double** m_itemQ;

   double* m_userBias;

   double* m_itemBias;

   size_t m_maxIter;

   double m_lambda;

   double m_learningRate;

   double m_decay;

   double m_loss;

   double m_prevLoss;

};

#endif // __ALG_FUNK_SVD_H__

