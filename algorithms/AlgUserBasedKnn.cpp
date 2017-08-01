#include "AlgUserBasedKnn.h"
#include "Similarity.h"
#include "MaxHeap.h"

using namespace std;


AlgUserBasedKnn::AlgUserBasedKnn( DataReader& dreader,
                                  int userpos,
                                  int itempos,
                                  int ratingpos )
: RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >( dreader, userpos, itempos, ratingpos ),
  m_knn( 10 ),
  m_pSimMatrix( NULL )
{
}

AlgUserBasedKnn::~AlgUserBasedKnn()
{
   if( NULL != m_pSimMatrix )
   {
      delete m_pSimMatrix;
   }
}

int AlgUserBasedKnn::train()
{
   string strSimType = "pearson";
   return train( 10, strSimType );
}

int AlgUserBasedKnn::train( size_t k, string& similarity )
{
   m_knn = k;
   Similarity<SparseRow< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> > > simfunc( similarity );
   size_t nusers = m_ratingMatrix.users();
   if( NULL != m_pSimMatrix )
   {
      delete m_pSimMatrix;
   }
   m_pSimMatrix = new SymmMatrix( nusers );
   for( size_t i = 0 ; i < nusers ; ++i )
   {
      // Mean rating matrix
      SparseRow< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major > > rowi = m_ratingMatrix.userVector( i );
      string userId = m_ratingMatrix.userId( i );
      m_meanRatingByUser[userId] = rowi.mean();

      // Similarity matrix
      for( size_t j = i + 1 ; j < nusers ; ++j )
      {
         SparseRow< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major > > rowj = m_ratingMatrix.userVector( j );
         double sim = simfunc.calculate( rowi, rowj );
         m_pSimMatrix->set( i, j, sim );

         if( !m_running )
         {
            return STOPPED;
         }
      }
   }

   return FINISHED;
}

double AlgUserBasedKnn::predict( string& userId, string& itemId )
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

double AlgUserBasedKnn::predict( size_t userrow, size_t itemcol )
{
   MaxHeap maxheap;

   double sum = 0;
   double ws = 0;
   int nusers = m_ratingMatrix.users();
   if( m_ratingMatrix.users() > userrow && m_ratingMatrix.items() > itemcol )
   {
      for( int i = 0 ; i < nusers ; ++i )
      {
         double sim = m_pSimMatrix->get( userrow, i );
         double rate = m_ratingMatrix.get( i, itemcol );
         if( rate > 0 && sim > 0 && i != userrow )
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

         double rate = m_ratingMatrix.get( idx, itemcol );
         string simUserId = m_ratingMatrix.userId( idx );

         sum += sim * ( rate - m_meanRatingByUser[simUserId] );
         ws += abs( sim );
      }
   }

   string userId = m_ratingMatrix.userId( userrow );
   return ws > 0 ? m_meanRatingByUser[userId] + sum / ws : m_globalMean;
}


