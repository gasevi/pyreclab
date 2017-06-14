#include "AlgItemBasedKnn.h"
#include "Similarity.h"
#include "MaxHeap.h"

using namespace std;


AlgItemBasedKnn::AlgItemBasedKnn( DataReader& dreader,
                                  int userpos,
                                  int itempos,
                                  int ratingpos )
: RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> >( dreader, userpos, itempos, ratingpos ),
  m_knn( 10 )
{
}

int AlgItemBasedKnn::train()
{  
   return train( 10 );
}

int AlgItemBasedKnn::train( size_t k )
{
   m_knn = k;
   Similarity< SparseColumn< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> > >simfunc( Similarity<SparseColumn< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> > >::PEARSON );
   size_t nitems = m_ratingMatrix.items();
   m_simMatrix.resize( nitems, nitems );
   for( int i = 0 ; i < nitems ; ++i )
   {
      // Mean rating matrix
      SparseColumn< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> > coli = m_ratingMatrix.itemVector( i );
      string itemId = m_ratingMatrix.itemId( i );
      m_meanRatingByItem[itemId] = coli.mean();

      // Similarity matrix
      for( int j = i + 1 ; j < nitems ; ++j )
      {
         SparseColumn< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> > colj = m_ratingMatrix.itemVector( j );
         double sim = simfunc.calculate( coli, colj );
         m_simMatrix.set( i, j, sim );
         m_simMatrix.set( j, i, sim );

         if( !m_running )
         {
            return STOPPED;
         }
      }
   }

   return FINISHED;
}

void AlgItemBasedKnn::test( DataFrame& dataFrame )
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

double AlgItemBasedKnn::predict( string userId, string itemId )
{
   MaxHeap maxheap;

   int userrow = m_ratingMatrix.row( userId );
   int itemcol = m_ratingMatrix.column( itemId );

   double sum = 0;
   double ws = 0;
   if( userrow >= 0 && itemcol >= 0 )
   {
      SparseRow< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> > row = m_simMatrix.row( itemcol );
      SparseRow< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >::iterator ind;
      SparseRow< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >::iterator end = row.end();
      for( ind = row.begin() ; ind != end ; ++ind )
      {
         double sim = *ind;
         double rate = m_ratingMatrix.get( userrow, ind.index() );
         if( sim > 0 && rate > 0 )
         {
            pair<double, size_t> e = pair<double, size_t>( sim, ind.index() );
            maxheap.push( e );
         }
      }

      for( int i = 0 ; i < m_knn ; ++i )
      {
         if( maxheap.empty() )
         {
            break;
         }
         pair<double, size_t> e = maxheap.pop();
         double sim = e.first;
         size_t idx = e.second;

         double rate = m_ratingMatrix.get( userrow, idx );
         string simItemId = m_ratingMatrix.itemId( idx );

         sum += sim * ( rate - m_meanRatingByItem[simItemId] );
         ws += abs( sim );
      }
   }

   return ws > 0 ? m_meanRatingByItem[itemId] + sum / ws : m_globalMean;
}


