#include "AlgItemAvg.h"
#include "ProgressBar.h"

using namespace std;


AlgItemAvg::AlgItemAvg( DataReader& dreader,
                        int userpos,
                        int itempos,
                        int ratingpos )
: RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> >( dreader, userpos, itempos, ratingpos )
{
   m_meanRatingByItemCol = new double[m_ratingMatrix.items()];
}

AlgItemAvg::~AlgItemAvg()
{
   if( NULL != m_meanRatingByItemCol )
   {
      delete m_meanRatingByItemCol;
   }
}

int AlgItemAvg::train( FlowControl& fcontrol, bool progress )
{
   size_t nitems = m_ratingMatrix.items();
   ProgressBar pbar( nitems, progress );
   for( size_t col = 0 ; col < nitems ; ++col )
   {
      double sumbycol = 0;
      int countbycol = m_ratingMatrix.sumColumn( col, sumbycol );
      if( 0 < countbycol )
      {
         m_meanRatingByItemCol[col] = sumbycol/countbycol;
      }

      if( fcontrol.interrupt() )
      {
         return STOPPED;
      }

      pbar.update( col + 1 );
   }

   return FINISHED;
}

double AlgItemAvg::predict( string& userId, string& itemId )
{
   double p = -1;
   int col = m_ratingMatrix.column( itemId );
   if( -1 < col )
   {
      p = m_meanRatingByItemCol[col];
   }
   return p > 0 ? p : m_globalMean;
}

double AlgItemAvg::predict( size_t userrow, size_t itemcol )
{
   double p = -1;
   if( itemcol < m_ratingMatrix.items() )
   {
      p = m_meanRatingByItemCol[itemcol];
   }
   return p > 0 ? p : m_globalMean;
}


