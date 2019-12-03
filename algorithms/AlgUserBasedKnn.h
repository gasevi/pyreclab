#ifndef __ALG_USER_BASED_KNN_H__
#define __ALG_USER_BASED_KNN_H__

#include "RecSysAlgorithm.h"
#include "SymmMatrix.h"
#include "FlowControl.h"

#include <string>
#include <stdexcept>


class AlgUserBasedKnn
      : public RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >
{
public:

   AlgUserBasedKnn( DataReader& dreader,
                    int userpos = 0,
                    int itempos = 1,
                    int ratingpos = 2 );

   ~AlgUserBasedKnn();

   int train( FlowControl& fcontrol, bool progress );

   int train( size_t k, std::string& similarity, FlowControl& fcontrol, bool progress );

   double predict( std::string& userId, std::string& itemId )
   throw( std::invalid_argument );

   double predict( size_t userrow, size_t itemcol );

private:

   size_t m_knn;

   double* m_meanRatingByUserRow;

   SymmMatrix* m_pSimMatrix;

};

#endif // __ALG_USER_BASED_KNN_H__

