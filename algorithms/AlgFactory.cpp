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


RecSysAlgorithm* AlgFactory::getInstance( std::string algorithm, RatingMatrix& ratingMatrix )
{
   RecSysAlgorithm* algInstance = NULL;
   transform( algorithm.begin(), algorithm.end(), algorithm.begin(), ::tolower );
   if( algorithm == "useravg" )
   {
      algInstance = new AlgUserAvg( ratingMatrix );
   }
   else if( algorithm == "itemavg" )
   {
      algInstance = new AlgItemAvg( ratingMatrix );
   }
   else if( algorithm == "slopeone" )
   {
      algInstance = new AlgSlopeOne( ratingMatrix );
   }
   else if( algorithm == "userknn" )
   {
      algInstance = new AlgUserBasedKnn( ratingMatrix );
   }
   else if( algorithm == "itemknn" )
   {
      algInstance = new AlgItemBasedKnn( ratingMatrix );
   }
   else if( algorithm == "svd" )
   {
      algInstance = new AlgFunkSvd( ratingMatrix );
   }
   else if( algorithm == "mostpopular" )
   {
      algInstance = new AlgMostPopular( ratingMatrix );
   }
   return algInstance;
}

