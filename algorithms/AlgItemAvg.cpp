#include "AlgItemAvg.h"

using namespace std;


AlgItemAvg::AlgItemAvg( RatingMatrix& ratingMatrix )
: RecSysAlgorithm( ratingMatrix )
{
}

int AlgItemAvg::train()
{
   for( int i = 0 ; i < m_ratingMatrix.items() ; ++i )
   {
      string itemId = m_ratingMatrix.itemId( i );
      double sumbycol = 0;
      int countbycol = m_ratingMatrix.sumColumn( i, sumbycol );
      if( 0 < countbycol )
      {
         m_meanRatingByItem[itemId] = sumbycol/countbycol;
      }

      if( !m_running )
      {
         return STOPPED;
      }
   }

   return FINISHED;
}

void AlgItemAvg::test( DataFrame& dataFrame )
{
   DataFrame::iterator ind;
   DataFrame::iterator end = dataFrame.end();
   for( ind = dataFrame.begin() ; ind != end ; ++ind )
   {
      string userId = ind->first.first;
      string itemId = ind->first.second;
      double prediction = predict( userId, itemId );
      //cout << userId << "," << itemId << "," << prediction << endl;
   }
}

double AlgItemAvg::predict( string userId, string itemId )
{
   double p = m_meanRatingByItem[itemId];
   return p > 0 ? p : m_globalMean;
}


