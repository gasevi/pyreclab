#ifndef __ALG_ITEM_BASED_KNN_H__
#define __ALG_ITEM_BASED_KNN_H__

#include "RecSysAlgorithm.h"
#include "SymmMatrix.h"
#include "FlowControl.h"

#include <string>
#include <stdexcept>


class AlgItemBasedKnn
      : public RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> >
{
public:

   AlgItemBasedKnn( DataReader& dreader,
                    int userpos = 0,
                    int itempos = 1,
                    int ratingpos = 2 );

   ~AlgItemBasedKnn();

   int train( FlowControl& fcontrol );

   int train( size_t k, std::string& similarity, FlowControl& fcontrol );

   void test( DataFrame& dataFrame );

   double predict( std::string& userId, std::string& itemId )
   throw( std::invalid_argument );

   double predict( size_t userrow, size_t itemcol );

private:

   size_t m_knn;

   double* m_meanRatingByItemCol;

   SymmMatrix* m_pSimMatrix;

};

#endif // __ALG_ITEM_BASED_KNN_H__

