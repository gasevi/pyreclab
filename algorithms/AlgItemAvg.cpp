#include "AlgItemAvg.h"

using namespace std;


AlgItemAvg::AlgItemAvg( DataReader& dreader,
                        int userpos,
                        int itempos,
                        int ratingpos )
: RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> >( dreader, userpos, itempos, ratingpos )
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

double AlgItemAvg::predict( string& userId, string& itemId )
{
   double p = m_meanRatingByItem[itemId];
   return p > 0 ? p : m_globalMean;
}

double AlgItemAvg::predict( size_t userrow, size_t itemcol )
{
   string itemId = m_ratingMatrix.itemId( itemcol );
   double p = m_meanRatingByItem[itemId];
   return p > 0 ? p : m_globalMean;
}


