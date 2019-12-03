#include "AlgUserAvg.h"
#include "ProgressBar.h"

using namespace std;


AlgUserAvg::AlgUserAvg( DataReader& dreader,
                        int userpos,
                        int itempos,
                        int ratingpos )
: RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >( dreader, userpos, itempos, ratingpos )
{
   m_meanRatingByUserRow = new double[m_ratingMatrix.users()];
}

AlgUserAvg::~AlgUserAvg()
{
   if( NULL != m_meanRatingByUserRow )
   {
      delete m_meanRatingByUserRow;
   }
}

int AlgUserAvg::train( FlowControl& fcontrol, bool progress )
{
   size_t nusers = m_ratingMatrix.users();
   ProgressBar pbar( nusers, progress );
   for( size_t row = 0 ; row < nusers ; ++row )
   {
      double sumbyrow = 0;
      int countbyrow = m_ratingMatrix.sumRow( row, sumbyrow );
      if( 0 < countbyrow )
      {
         m_meanRatingByUserRow[row] = sumbyrow/countbyrow;
      }

      if( fcontrol.interrupt() )
      {
         return STOPPED;
      }

      pbar.update( row + 1 );
   }

   return FINISHED;
}

double AlgUserAvg::predict( string& userId, string& itemId )
{
   double p = -1;
   int row = m_ratingMatrix.row( userId );
   if( -1 < row )
   {
      p = m_meanRatingByUserRow[row];
   }
   return p > 0 ? p : m_globalMean;
}

double AlgUserAvg::predict( size_t userrow, size_t itemcol )
{
   double p = -1;
   string userId = m_ratingMatrix.userId( userrow );
   if( userrow < m_ratingMatrix.users() )
   {
      p = m_meanRatingByUserRow[userrow];
   }
   return p > 0 ? p : m_globalMean;
}


