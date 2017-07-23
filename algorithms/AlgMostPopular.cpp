#include "AlgMostPopular.h"
#include "MaxHeap.h"

using namespace std;


AlgMostPopular::AlgMostPopular( DataReader& dreader,
                                int userpos,
                                int itempos,
                                int ratingpos )
: RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >( dreader, userpos, itempos, ratingpos )
{
}

int AlgMostPopular::train()
{
   for( size_t i = 0 ; i < m_ratingMatrix.items() ; ++i )
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

bool AlgMostPopular::recommend( const std::string& userId,
                                size_t n,
                                std::vector<std::string>& ranking,
                                bool includeRated )
{
   int row = m_ratingMatrix.row( userId );
   size_t nitems = m_itemList.size();
   for( size_t col = 0 ; col < nitems ; ++col )
   {
      if( includeRated || ( 0 == m_ratingMatrix.get( row, col ) ) )
      {
         size_t itemcol = m_itemList[col].second;
         string itemId = m_ratingMatrix.itemId( itemcol );
         ranking.push_back( itemId );
         if( ranking.size() == n )
         {
            break;
         }
      }
   }

   return true;
}


