#ifndef __RECSYS_ALGORITHM_H__
#define __RECSYS_ALGORITHM_H__

#include "RatingMatrix.h"
#include "DataFrame.h"

#include <string>


class RecSysAlgorithm
{
public:

   RecSysAlgorithm( RatingMatrix& ratingMatrix )
   : m_ratingMatrix( ratingMatrix )
   {
      m_globalMean = m_ratingMatrix.sumRatings()/m_ratingMatrix.numRatings();
   }

   virtual
   void train() = 0;

   virtual
   void test( DataFrame& dataFrame ) = 0;

   virtual
   double predict( std::string userId, std::string itemId )
   {
      return 0;
   }

   virtual
   bool rank( const std::string& userId, std::vector<std::string>& rank )
   {
      return false;
   }

protected:

   RatingMatrix& m_ratingMatrix;

   double m_globalMean;

};

#endif // __RECSYS_ALGORITHM_H__

