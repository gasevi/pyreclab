#include "AlgUserAvg.h"

using namespace std;


AlgUserAvg::AlgUserAvg( DataReader& dreader,
                        int userpos,
                        int itempos,
                        int ratingpos )
: RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >( dreader, userpos, itempos, ratingpos )
{
}

int AlgUserAvg::train()
{
   for( int u = 0 ; u < m_ratingMatrix.users() ; ++u )
   {
      string userId = m_ratingMatrix.userId( u );
      double sumbyrow = 0;
      int countbyrow = m_ratingMatrix.sumRow( u, sumbyrow );
      if( 0 < countbyrow )
      {
         m_meanRatingByUser[userId] = sumbyrow/countbyrow;
      }

      if( !m_running )
      {
         return STOPPED;
      }
   }

   return FINISHED;
}

void AlgUserAvg::test( DataFrame& dataFrame )
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

double AlgUserAvg::predict( string userId, string itemId )
{
   double p = m_meanRatingByUser[userId];
   return p > 0 ? p : m_globalMean;
}


