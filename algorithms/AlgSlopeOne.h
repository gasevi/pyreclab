#ifndef __ALG_SLOPE_ONE_H__
#define __ALG_SLOPE_ONE_H__

#include "RecSysAlgorithm.h"

#include <string>


class AlgSlopeOne
      : public RecSysAlgorithm
{
public:

   AlgSlopeOne( RatingMatrix& ratingMatrix );

   void train();

   void test( DataFrame& dataFrame );

   double predict( std::string userId, std::string itemId );
 
private:

   SparseMatrix m_devMatrix;

   SparseMatrix m_cardMatrix;

};

#endif // __ALG_SLOPE_ONE_H__

