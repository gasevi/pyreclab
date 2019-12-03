#ifndef __ALG_SLOPE_ONE_H__
#define __ALG_SLOPE_ONE_H__

#include "RecSysAlgorithm.h"
#include "FlowControl.h"

#include <string>
#include <stdexcept>

#include "DenseMatrix.h"


class AlgSlopeOne
      : public RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >
{
public:

   AlgSlopeOne( DataReader& dreader,
                int userpos = 0,
                int itempos = 1,
                int ratingpos = 2 );

   ~AlgSlopeOne();

   int train( FlowControl& fcontrol, bool progress );

   double predict( std::string& userId, std::string& itemId )
   throw( std::invalid_argument );

   double predict( size_t userrow, size_t itemcol );
 
private:

   DenseMatrix* m_pDevMatrix;

   DenseMatrix* m_pCardMatrix;

};

#endif // __ALG_SLOPE_ONE_H__

