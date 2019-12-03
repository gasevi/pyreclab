#ifndef __ALG_BPR_MF_H__
#define __ALG_BPR_MF_H__

#include "RecSysAlgorithm.h"
#include "FlowControl.h"

#include <string>
#include <stdexcept>
#include <utility>


class AlgBprMf
      : public RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >
{
public:

   AlgBprMf( size_t factors,
             DataReader& dreader,
             int userpos = 0,
             int itempos = 1,
             int ratingpos = 2 );

   ~AlgBprMf();

   int train( size_t maxiter,
              float lrate,
              float lambdaW,
              float lambdaHp,
              float lambdaHm,
              FlowControl& fcontrol,
              bool progress );

   void reset();

   double predict( size_t userrow, size_t itemcol );

   double loss();

private:

   int train( FlowControl& fcontrol, bool progress );

   double score( size_t userrow, size_t itemcoli, size_t itemcolj );

   void sample( int& u, int& i, int& j );

   double innerProduct( double v1[], double v2[], size_t size );

   size_t m_nfactors;

   std::vector<int>** m_pObservedItemsIndices;

   double** m_userP;

   double** m_itemQ;

   size_t m_maxIter;

   double m_lambdaW;

   double m_lambdaHp;

   double m_lambdaHm;

   double m_lr;

};

#endif // __ALG_BPR_MF_H__

