#ifndef __ALG_FUNK_SVD_H__
#define __ALG_FUNK_SVD_H__

#include "RecSysAlgorithm.h"
#include "FlowControl.h"

#include <string>
#include <stdexcept>


class AlgFunkSvd
      : public RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >
{
public:

   AlgFunkSvd( DataReader& dreader,
               int userpos = 0,
               int itempos = 1,
               int ratingpos = 2 );

   ~AlgFunkSvd();

   int train( FlowControl& fcontrol );

   int train( size_t factors, size_t maxiter, float lrate, float lambda, FlowControl& fcontrol );

   double predict( std::string& userId, std::string& itemId )
   throw( std::invalid_argument );

   double predict( size_t userrow, size_t itemcol );

private:

   void reset( size_t factors, size_t maxiter, float lrate, float lambda );

   bool converged( double loss );

   void adaptLR( double iter );

   double innerProduct( double v1[], double v2[], size_t size );

   size_t m_nfactors;

   double** m_userP;

   double** m_itemQ;

   double* m_userBias;

   double* m_itemBias;

   size_t m_maxIter;

   double m_lambda;

   double m_learningRate;

   double m_decay;

   double m_prevLoss;

};

#endif // __ALG_FUNK_SVD_H__

