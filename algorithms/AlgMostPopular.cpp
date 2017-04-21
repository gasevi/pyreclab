#include "AlgMostPopular.h"
#include "MaxHeap.h"

using namespace std;


AlgMostPopular::AlgMostPopular( RatingMatrix& ratingMatrix )
: RecSysAlgorithm( ratingMatrix )
{
}

int AlgMostPopular::train()
{
   for( int i = 0 ; i < m_ratingMatrix.items() ; ++i )
   {
      double score = m_ratingMatrix.nonZeroCol( i );
      pair<double, size_t> e = pair<double, size_t>( score, i );
      m_itemList.push_back( e );

      if( !m_running )
      {
         sort( m_itemList.rbegin(), m_itemList.rend() );
         return STOPPED;
      }
   }
   sort( m_itemList.rbegin(), m_itemList.rend() );

   return FINISHED;
}

void AlgMostPopular::test( DataFrame& dataFrame )
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

bool AlgMostPopular::recommend( const std::string& userId, size_t n, std::vector<std::string>& ranking )
{
   for( int i = 0 ; i < n ; ++i )
   {
      if( m_itemList.size() <= i )
      {
         break;
      }
      size_t itemcol = m_itemList[i].second;
      string itemId = m_ratingMatrix.itemId( itemcol );
      ranking.push_back( itemId );
   }

   return true;
}


