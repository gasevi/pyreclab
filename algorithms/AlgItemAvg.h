#ifndef __ALG_ITEM_AVERAGE_H__
#define __ALG_ITEM_AVERAGE_H__

#include "RecSysAlgorithm.h"

#include <string>
#include <map>


class AlgItemAvg
      : public RecSysAlgorithm
{
public:

   AlgItemAvg( RatingMatrix& ratingMatrix );

   void train();

   void test( DataFrame& dataFrame );

   double predict( std::string userId, std::string itemId );
 
private:

   std::map<std::string, double> m_meanRatingByItem;

};

#endif // __ALG_ITEM_AVERAGE_H__

