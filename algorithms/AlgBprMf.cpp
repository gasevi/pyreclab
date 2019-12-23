#include "AlgBprMf.h"
#include "Similarity.h"
#include "MaxHeap.h"
#include "NormalDistribution.h"
#include "ProgressBar.h"

#include <cmath>

using namespace std;


AlgBprMf::AlgBprMf( size_t factors,
                    DataReader& dreader,
                    int userpos,
                    int itempos,
                    int ratingpos )
: RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >( dreader, userpos, itempos, ratingpos ),
  m_uniform_dist_users( 0, m_ratingMatrix.users() - 1 ),
  m_uniform_dist_items( 0, m_ratingMatrix.items() - 1 ),
  m_nfactors( factors ),
  m_userP( NULL ),
  m_itemQ( NULL ),
  m_maxIter( 1 ),
  m_lambdaW( 0.01 ),
  m_lambdaHp( 0.01 ),
  m_lambdaHm( 0.01 ),
  m_lr( 0.1 )
{
   size_t nusers = m_ratingMatrix.users();
   size_t nitems = m_ratingMatrix.items();

   m_pObservedItemsIndices = new vector<int>*[nusers];
   for( size_t u = 0 ; u < nusers ; ++u )
   {
      m_pObservedItemsIndices[u] = new vector<int>();
      SparseRow< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> > row = m_ratingMatrix.userVector( u );
      SparseRow< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >::iterator ind;
      SparseRow< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >::iterator end = row.end();
      for( ind = row.begin() ; ind != end ; ++ind )
      {
         m_pObservedItemsIndices[u]->push_back( ind.index() );
      }
   }

   NormalDistribution normal_dist( 0, 0.1 );

   m_userP = new double*[nusers];
   for( size_t i = 0 ; i < nusers ; ++i )
   {
      m_userP[i] = new double[m_nfactors];
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_userP[i][f] = normal_dist();
      }
   }

   m_itemQ = new double*[nitems];
   for( size_t i = 0 ; i < nitems ; ++i )
   {
      m_itemQ[i] = new double[m_nfactors];
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_itemQ[i][f] = normal_dist();
      }
   }
}

AlgBprMf::~AlgBprMf()
{
   size_t nusers = m_ratingMatrix.users();
   for( size_t u = 0 ; u < nusers ; ++u )
   {
      delete m_pObservedItemsIndices[u];
   }
   delete [] m_pObservedItemsIndices;

   if( NULL != m_userP )
   {
      size_t nusers = m_ratingMatrix.users();
      for( size_t i = 0 ; i < nusers ; ++i )
      {
         delete [] m_userP[i];
      }
      delete [] m_userP;
   }

   if( NULL != m_itemQ )
   {
      size_t nitems = m_ratingMatrix.items();
      for( size_t i = 0 ; i < nitems ; ++i )
      {
         delete [] m_itemQ[i];
      }
      delete [] m_itemQ;
   }
}

int AlgBprMf::train( size_t maxiter, float lrate, float lambdaW, float lambdaHp, float lambdaHm, FlowControl& fcontrol, bool progress )
{
   m_maxIter = maxiter;
   m_lr = lrate;
   m_lambdaW = lambdaW;
   m_lambdaHp = lambdaHp;
   m_lambdaHm = lambdaHm;

   return train( fcontrol, progress );
}

int AlgBprMf::train( FlowControl& fcontrol, bool progress )
{
   ProgressBar pbar( m_maxIter, progress );
   for( size_t it = 0 ; it < m_maxIter ; ++it )
   {
      // Draw (u,i,j) from Ds
      int u = 0;
      int i = 0;
      int j = 0;
      sample( u, i, j );
      // Update rule
      double xuij = score( u, i, j );
      double expnxuij = exp( -xuij );
      double dsigmoid = expnxuij / ( 1 + expnxuij );
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         double wuf = m_userP[u][f];
         double hif = m_itemQ[i][f];
         double hjf = m_itemQ[j][f];

         double grad_u = dsigmoid * (hif - hjf) - m_lambdaW * wuf;
         double grad_i = dsigmoid * wuf - m_lambdaHp * hif;
         double grad_j = dsigmoid * -wuf - m_lambdaHm * hjf;

         // update user and items matrices
         m_userP[u][f] += m_lr * grad_u;
         m_itemQ[i][f] += m_lr * grad_i;
         m_itemQ[j][f] += m_lr * grad_j;

         if( fcontrol.interrupt() )
         {
            return STOPPED;
         }
      }
      pbar.update( it + 1 );
   }

   return FINISHED;
}

void AlgBprMf::sample( int& u, int& i, int& j )
{
   u = m_uniform_dist_users();
   size_t numObserved = m_pObservedItemsIndices[u]->size();
   // numObserved: be carefull with items rated with value 0

   UniformIntDistribution uniform_dist_obs_items( 0, numObserved - 1 );
   int obsidx = uniform_dist_obs_items();
   i = (*m_pObservedItemsIndices[u])[obsidx];

   j = m_uniform_dist_items();
   double observation = m_ratingMatrix.get( u, j );
   while( observation != 0 )
   {
      j = m_uniform_dist_items();
      observation = m_ratingMatrix.get( u, j );
   }
}

double AlgBprMf::predict( size_t userrow, size_t itemcol )
{
   double pred = m_globalMean;
   size_t nusers = m_ratingMatrix.users();
   size_t nitems = m_ratingMatrix.items();
   if( ( userrow < nusers ) && ( itemcol < nitems ) )
   {
      double* pu = m_userP[userrow];
      double* qi = m_itemQ[itemcol];
      pred = innerProduct( pu, qi, m_nfactors );
   }
   return pred;
}

double AlgBprMf::score( size_t userrow, size_t itemcoli, size_t itemcolj )
{
   double pred = m_globalMean;
   size_t nusers = m_ratingMatrix.users();
   size_t nitems = m_ratingMatrix.items();
   if( ( userrow < nusers ) && ( itemcoli < nitems ) && ( itemcolj < nitems ) )
   {
      pred = 0;
      double* pu = m_userP[userrow];
      double* qi = m_itemQ[itemcoli];
      double* qj = m_itemQ[itemcolj];
      for( size_t k = 0 ; k < m_nfactors ; ++k )
      {
         pred += pu[k] * ( qi[k] - qj[k] );
      }
   }
   return pred;
}

void AlgBprMf::reset()
{
   size_t nusers = m_ratingMatrix.users();
   size_t nitems = m_ratingMatrix.items();

   NormalDistribution normal_dist( 0, 0.1 );

   for( size_t i = 0 ; i < nusers ; ++i )
   {
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_userP[i][f] = normal_dist();
      }
   }

   for( size_t i = 0 ; i < nitems ; ++i )
   {
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_itemQ[i][f] = normal_dist();
      }
   }
}

double AlgBprMf::loss()
{
   double loss = 0;
   size_t nusers = m_ratingMatrix.users();
   size_t nitems = m_ratingMatrix.items();
   double lambdaH = ( m_lambdaHp + m_lambdaHm )/2;

   for( size_t it = 0 ; it < 1000 ; ++it )
   {
      // Draw (u,i,j) from Ds
      int u = 0;
      int i = 0;
      int j = 0;
      sample( u, i, j );
      // Update rule
      double xuij = score( u, i, j );
      double sigmoid = 1 / ( 1 + exp( -xuij ) );
      loss += log( sigmoid );
   }

   for( size_t f = 0 ; f < m_nfactors ; ++f )
   {
      for( size_t u = 0 ; u < nusers ; ++u )
      {
         loss += m_lambdaW * m_userP[u][f];
      }
      for( size_t i = 0 ; i < nitems ; ++i )
      {
         loss += lambdaH * m_itemQ[i][f];
      }
   }

   return loss;
}

double AlgBprMf::innerProduct( double v1[], double v2[], size_t size )
{
   double sum = 0;
   for( size_t i = 0 ; i < size ; ++i )
   {
      sum += v1[i] * v2[i];
   }
   return sum;
}


