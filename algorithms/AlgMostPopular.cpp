#include "AlgMostPopular.h"
#include "MaxHeap.h"

using namespace std;


AlgMostPopular::AlgMostPopular( RatingMatrix& ratingMatrix )
: RecSysAlgorithm( ratingMatrix ),
  m_topn( 10 )
{
}

int AlgMostPopular::train( size_t topn )
{
   m_topn = topn;
   return train();
}

int AlgMostPopular::train()
{
   for( int i = 0 ; i < m_ratingMatrix.items() ; ++i )
   {
      double score = m_ratingMatrix.nonZeroCol( i );
      if( m_itemList.size() < m_topn )
      {
         pair<double, size_t> e = pair<double, size_t>( score, i );
         m_itemList.push_back( e );
         sort( m_itemList.rbegin(), m_itemList.rend() );
      }
      else if( m_itemList.back().first < score )
      {
         pair<double, size_t> e = pair<double, size_t>( score, i );
         m_itemList.push_back( e );
         sort( m_itemList.rbegin(), m_itemList.rend() );
         m_itemList.pop_back();
      }

      if( !m_running )
      {
         return STOPPED;
      }
   }

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

bool AlgMostPopular::rank( const string& userId, vector<string>& itemList )
{
   bool ret = false;
   if( m_itemList.size() == m_topn )
   {
      for( int i = 0 ; i < m_topn ; ++i )
      {
         size_t itemcol = m_itemList[i].second;
         string itemId = m_ratingMatrix.itemId( itemcol );
         itemList.push_back( itemId );
      }
      ret = true;
   }

   return ret;
}


