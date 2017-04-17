#include "AlgSlopeOne.h"

using namespace std;


AlgSlopeOne::AlgSlopeOne( RatingMatrix& ratingMatrix )
: RecSysAlgorithm( ratingMatrix )
{
   size_t nitems = m_ratingMatrix.items();
   m_devMatrix.resize( nitems, nitems );
   m_cardMatrix.resize( nitems, nitems );
}

int AlgSlopeOne::train()
{
   double ratesum = 0;
   size_t nusers = m_ratingMatrix.users();
   size_t nitems = m_ratingMatrix.items();
   int porcent = -1;

   for( int u = 0 ; u < nusers ; ++u )
   {
      SparseRow row = m_ratingMatrix.userVector( u );
      SparseRow::iterator ind1;
      SparseRow::iterator end = row.end();
      for( ind1 = row.begin() ; ind1 != end ; ++ind1 )
      {
         double ru1 = *ind1;
         SparseRow::iterator ind2;
         SparseRow::iterator end2 = row.end();
         for( ind2 = row.begin() ; ind2 != end2 ; ++ind2 )
         {
            if( ind1 != ind2 )
            {
               size_t i = ind1.index();
               size_t j = ind2.index();
               double ru2 = *ind2;
               double diff = ru1 - ru2;
               m_devMatrix.add( i, j, diff );
               m_cardMatrix.add( i, j, 1 );
            }
/*
            if( static_cast<int>( 100*( u*nitems + ind1.index() ) / ( nitems*nusers ) ) > porcent )
            {
               porcent = 100*( u*nitems + ind1.index() ) / ( nitems*nusers );
               cout << porcent << " %" << endl;
            }
*/
            if( !m_running )
            {
               return STOPPED;
            }
         }

      }
   }

   for( int i = 0 ; i < nitems ; ++i )
   {
      for( int j = 0 ; j < nitems ; ++j )
      {
         double card = m_cardMatrix.get( i, j );
         if( card > 0 )
         {
            double sum = m_devMatrix.get( i, j );
            m_devMatrix.set( i, j, sum / card );
         }

         if( !m_running )
         {
            return STOPPED;
         }
      }
   }

   return FINISHED;
}

void AlgSlopeOne::test( DataFrame& dataFrame )
{
   DataFrame::iterator ind;
   DataFrame::iterator end = dataFrame.end();
   for( ind = dataFrame.begin() ; ind != end ; ++ind )
   {
      string userId = ind->first.first;
      string itemId = ind->first.second;
      double prediction = predict( userId, itemId );
   }
}

double AlgSlopeOne::predict( string userId, string itemId )
{
   double sumpred = 0;
   double sumcard = 0;
   int col = m_ratingMatrix.column( itemId );
   if( col >= 0 )
   {
      SparseRow row = m_ratingMatrix.userVector( userId );
      SparseRow::iterator ind;
      SparseRow::iterator end = row.end();
      for( ind = row.begin() ; ind != end ; ++ind )
      {
         size_t itempos = ind.index();
         if( col != itempos )
         {
            double rating = *ind;
            double card = m_cardMatrix.get( col, itempos );
            if( card > 0 )
            {
               sumpred += ( m_devMatrix.get( col, itempos ) + rating )*card;
               sumcard += card;
            }
         }
      }
   }

   double rate = 0;
   if( sumpred > 0 )
   {
      rate = sumpred / sumcard;
      if( rate > 5 )
      {
         rate = 5;
      }
      else if( rate < 1 )
      {
         rate = 1;
      }
   }
   else
   {
      rate = m_globalMean;
   }

   return rate;
}


