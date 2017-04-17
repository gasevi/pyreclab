#ifndef __ALG_USER_AVERAGE_H__
#define __ALG_USER_AVERAGE_H__

#include "RecSysAlgorithm.h"

#include <string>
#include <map>


class AlgUserAvg
      : public RecSysAlgorithm
{
public:

   AlgUserAvg( RatingMatrix& ratingMatrix );

   int train();

   void test( DataFrame& dataFrame );

   double predict( std::string userId, std::string itemId );
 
private:

   std::map<std::string, double> m_meanRatingByUser;

};

#endif // __ALG_USER_AVERAGE_H__

