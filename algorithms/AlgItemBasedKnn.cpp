#include "AlgItemBasedKnn.h"
#include "Similarity.h"
#include "MaxHeap.h"

using namespace std;


AlgItemBasedKnn::AlgItemBasedKnn( DataReader& dreader,
                                  int userpos,
                                  int itempos,
                                  int ratingpos )
: RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> >( dreader, userpos, itempos, ratingpos ),
  m_knn( 10 ),
  m_pSimMatrix( NULL )
{
}

AlgItemBasedKnn::~AlgItemBasedKnn()
{
   if( NULL != m_pSimMatrix )
   {
      delete m_pSimMatrix;
   }
}

int AlgItemBasedKnn::train()
{  
   string strSimType = "pearson";
   return train( 10, strSimType );
}

int AlgItemBasedKnn::train( size_t k, string& similarity )
{
   m_knn = k;
   Similarity< SparseColumn< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> > >simfunc( similarity );
   size_t nitems = m_ratingMatrix.items();
   if( NULL != m_pSimMatrix )
   {
      delete m_pSimMatrix;
   }
   m_pSimMatrix = new SymmMatrix( nitems );
   for( size_t i = 0 ; i < nitems ; ++i )
   {
      // Mean rating matrix
      SparseColumn< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> > coli = m_ratingMatrix.itemVector( i );
      string itemId = m_ratingMatrix.itemId( i );
      m_meanRatingByItem[itemId] = coli.mean();

      // Similarity matrix
      for( size_t j = i + 1 ; j < nitems ; ++j )
      {
         SparseColumn< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> > colj = m_ratingMatrix.itemVector( j );
         double sim = simfunc.calculate( coli, colj );
         m_pSimMatrix->set( i, j, sim );

         if( !m_running )
         {
            return STOPPED;
         }
      }
   }

   return FINISHED;
}

double AlgItemBasedKnn::predict( string& userId, string& itemId )
throw( invalid_argument )
{
   int userrow = m_ratingMatrix.row( userId );
   if( userrow < 0 )
   {
      std::ostringstream eMsg;
      eMsg << "user id '" << userId << "' was not included in training set";
      throw invalid_argument( eMsg.str() );
   }
   int itemcol = m_ratingMatrix.column( itemId );
   if( itemcol < 0 )
   {
      std::ostringstream eMsg;
      eMsg << "item id '" << itemId << "' was not included in training set";
      throw invalid_argument( eMsg.str() );
   }
   return predict( userrow, itemcol );
}

double AlgItemBasedKnn::predict( size_t userrow, size_t itemcol )
{
   MaxHeap maxheap;

   double sum = 0;
   double ws = 0;
   int nitems = m_ratingMatrix.items();
   if( m_ratingMatrix.users() > userrow && m_ratingMatrix.items() > itemcol )
   {
      for( int i = 0 ; i < nitems ; ++i )
      {
         double sim = m_pSimMatrix->get( itemcol, i );
         double rate = m_ratingMatrix.get( userrow, i );
         if( rate > 0 && sim > 0 && i != itemcol )
         {
            pair<double, size_t> e = pair<double, size_t>( sim, i );
            maxheap.push( e );
         }
      }

      for( size_t i = 0 ; i < m_knn ; ++i )
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

   string itemId = m_ratingMatrix.itemId( itemcol );
   return ws > 0 ? m_meanRatingByItem[itemId] + sum / ws : m_globalMean;
}


