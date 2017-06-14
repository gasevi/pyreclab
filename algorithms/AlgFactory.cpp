#include "AlgFactory.h"
#include "AlgUserAvg.h"
#include "AlgItemAvg.h"
#include "AlgSlopeOne.h"
#include "AlgUserBasedKnn.h"
#include "AlgItemBasedKnn.h"
#include "AlgFunkSvd.h"
#include "AlgMostPopular.h"

#include <algorithm>

using namespace std;


RecSysAlgorithm* AlgFactory::getInstance( std::string algorithm,
                                          DataReader& dreader,
                                          int userpos,
                                          int itempos,
                                          int ratingpos )
{
   RecSysAlgorithm* algInstance = NULL;
   transform( algorithm.begin(), algorithm.end(), algorithm.begin(), ::tolower );
   if( algorithm == "useravg" )
   {
      algInstance = new AlgUserAvg( dreader, userpos, itempos, ratingpos );
   }
   else if( algorithm == "itemavg" )
   {
      algInstance = new AlgItemAvg( dreader, userpos, itempos, ratingpos );
   }
   else if( algorithm == "slopeone" )
   {
      algInstance = new AlgSlopeOne( dreader, userpos, itempos, ratingpos );
   }
   else if( algorithm == "userknn" )
   {
      algInstance = new AlgUserBasedKnn( dreader, userpos, itempos, ratingpos );
   }
   else if( algorithm == "itemknn" )
   {
      algInstance = new AlgItemBasedKnn( dreader, userpos, itempos, ratingpos );
   }
   else if( algorithm == "svd" )
   {
      algInstance = new AlgFunkSvd( dreader, userpos, itempos, ratingpos );
   }
   else if( algorithm == "mostpopular" )
   {
      algInstance = new AlgMostPopular( dreader, userpos, itempos, ratingpos );
   }
   return algInstance;
}

