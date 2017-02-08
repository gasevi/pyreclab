#ifndef __ALGORITHM_FACTORY_H__
#define __ALGORITHM_FACTORY_H__

#include "RecSysAlgorithm.h"


class AlgFactory
{
public:

   static
   RecSysAlgorithm* getInstance( std::string algorithm, RatingMatrix& ratingMatrix );

};

#endif // __ALGORITHM_FACTORY_H__

