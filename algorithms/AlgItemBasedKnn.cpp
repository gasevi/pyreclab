#include "AlgItemBasedKnn.h"
#include "Similarity.h"
#include "MaxHeap.h"
#include "ProgressBar.h"

using namespace std;


AlgItemBasedKnn::AlgItemBasedKnn( DataReader& dreader,
                                  int userpos,
                                  int itempos,
                                  int ratingpos )
: RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> >( dreader, userpos, itempos, ratingpos ),
  m_knn( 10 ),
  m_pSimMatrix( NULL )
{
   m_meanRatingByItemCol = new double[m_ratingMatrix.items()];
}

AlgItemBasedKnn::~AlgItemBasedKnn()
{
   if( NULL != m_pSimMatrix )
   {
      delete m_pSimMatrix;
   }

   if( NULL != m_meanRatingByItemCol )
   {
      delete m_meanRatingByItemCol;
   }
}

int AlgItemBasedKnn::train( FlowControl& fcontrol, bool progress )
{  
   string strSimType = "pearson";
   return train( 10, strSimType, fcontrol, progress );
}

int AlgItemBasedKnn::train( size_t k, string& similarity, FlowControl& fcontrol, bool progress )
{
   m_knn = k;
   size_t nitems = m_ratingMatrix.items();
   Similarity< SparseColumn< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> > >simfunc( similarity );

   if( NULL != m_pSimMatrix )
   {
      delete m_pSimMatrix;
   }
   m_pSimMatrix = new SymmMatrix( nitems );

   ProgressBar pbar( nitems, progress );
   for( size_t i = 0 ; i < nitems ; ++i )
   {
      // Mean rating matrix
      SparseColumn< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> > coli = m_ratingMatrix.itemVector( i );
      m_meanRatingByItemCol[i] = coli.mean();

      // Similarity matrix
      for( size_t j = i + 1 ; j < nitems ; ++j )
      {
         SparseColumn< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::column_major> > colj = m_ratingMatrix.itemVector( j );
         double sim = simfunc.calculate( coli, colj );
         m_pSimMatrix->set( i, j, sim );

         if( fcontrol.interrupt() )
         {
            return STOPPED;
         }
      }

      pbar.update( i + 1 );
   }

   return FINISHED;
}

double AlgItemBasedKnn::predict( string& userId, string& itemId )
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
   size_t nitems = m_ratingMatrix.items();
   if( m_ratingMatrix.users() > userrow && m_ratingMatrix.items() > itemcol )
   {
      for( size_t i = 0 ; i < nitems ; ++i )
      {
         double sim = m_pSimMatrix->get( itemcol, i );
         double rate = m_ratingMatrix.get( userrow, i );
         if( rate > 0 && sim > 0 && i != itemcol )
         {
            pair<double, size_t> e = pair<double, size_t>( sim, i );
            maxheap.push( e );
         }
      }

      for( size_t k = 0 ; k < m_knn ; ++k )
      {
         if( maxheap.empty() )
         {
            break;
         }
         pair<double, size_t> e = maxheap.pop();
         double sim = e.first;
         size_t idx = e.second;

         double rate = m_ratingMatrix.get( userrow, idx );

         sum += sim * ( rate - m_meanRatingByItemCol[idx] );
         ws += abs( sim );
      }
   }

   return ws > 0 ? m_meanRatingByItemCol[itemcol] + sum / ws : m_globalMean;
}


