#include "RecSysAlgorithm.h"
#include "MaxHeap.h"

#include <utility>

using namespace std;

double RecSysAlgorithm::predict( std::string userId, std::string itemId )
{
   int userrow = m_ratingMatrix.row( userId );
   int itemcol = m_ratingMatrix.column( itemId );
   return predict( userrow, itemcol );
}

double RecSysAlgorithm::predict( size_t row, size_t col )
{
   return 0;
}

bool RecSysAlgorithm::recommend( const std::string& userId, size_t n, std::vector<std::string>& ranking )
{
   int row = m_ratingMatrix.row( userId );
   MaxHeap maxheap;
   for( size_t col = 0 ; col < m_ratingMatrix.items() ; ++col )
   {
      if( 0 == m_ratingMatrix.get( row, col ) )
      {
         double rating = predict( row, col );
         pair<double, size_t> e = pair<double, size_t>( rating, col );
         maxheap.push( e );
      }
   }

   if( maxheap.empty() )
   {
      return false;
   }

   for( int i = 0 ; i < n ; ++i )
   {
      pair<double, size_t> e = maxheap.pop();
      ranking.push_back( m_ratingMatrix.itemId( e.second ) );
   }

   return true;
}

