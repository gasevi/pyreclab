#ifndef __ALGORITHM_FACTORY_H__
#define __ALGORITHM_FACTORY_H__

#include "RecSysAlgorithm.h"


class AlgFactory
{
public:

   static
   RecSysAlgorithm* getInstance( std::string algorithm,
                                 DataReader& dreader,
                                 int userpos = 0,
                                 int itempos = 1,
                                 int ratingpos = 2 );

};

#endif // __ALGORITHM_FACTORY_H__

