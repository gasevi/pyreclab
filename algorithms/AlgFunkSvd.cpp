#include "AlgFunkSvd.h"
#include "Similarity.h"
#include "MaxHeap.h"

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

using namespace std;
using namespace boost;


AlgFunkSvd::AlgFunkSvd( RatingMatrix& ratingMatrix )
: RecSysAlgorithm( ratingMatrix ),
  m_nfactors( 1000 ),
  m_maxIter( 100 ),
  m_learningRate( 0.01 ),
  m_decay( -1 ),
  m_lambda( 0.1 ),
  m_prevLoss( 0 ),
  m_userP( NULL ),
  m_itemQ( NULL ),
  m_userBias( NULL ),
  m_itemBias( NULL )
{
}

AlgFunkSvd::~AlgFunkSvd()
{
   for( int i = 0 ; i < m_nfactors ; ++i )
   {
      delete [] m_userP[i];
      delete [] m_itemQ[i];
   }
   delete [] m_userP;
   delete [] m_itemQ;
   delete [] m_userBias;
   delete [] m_itemBias;
}

void AlgFunkSvd::train( size_t factors, size_t maxiter, float lrate, float lambda )
{
   reset( factors, maxiter, lrate, lambda );
   train();
}

void AlgFunkSvd::train()
{
   for( int it = 0 ; it < m_maxIter ; ++it )
   {
      double loss = 0;
      size_t nusers = m_ratingMatrix.users();
      for( int u = 0 ; u < nusers ; ++u )
      {
         SparseRow row = m_ratingMatrix.userVector( u );
         SparseRow::iterator ind;
         SparseRow::iterator end = row.end();
         for( ind = row.begin() ; ind != end ; ++ind )
         {
            int i = ind.index();

            double pred = predict( u, i );
            double eui = *ind - pred;

            // squared error: ( rui - ( mu + bu + bi + pu*qi ) )^2
            loss += eui * eui;

            double bu = m_userBias[u];
            double bi = m_itemBias[i];

            // bias regularization: lambda * ( bu^2 + bi^2 )
            loss += m_lambda * bu * bu;
            loss += m_lambda * bi * bi;

            // update biases
            double buNext = eui - m_lambda * bu;
            m_userBias[u] += m_learningRate * buNext;
            double biNext = eui - m_lambda * bi;
            m_itemBias[i] += m_learningRate * biNext;

            for( int f = 0 ; f < m_nfactors ; ++f )
            {
               double puf = m_userP[u][f];
               double qif = m_itemQ[i][f];

               double delta_u = eui * qif - m_lambda * puf;
               double delta_i = eui * puf - m_lambda * qif;

               // update user and item matrices
               m_userP[u][f] += m_learningRate * delta_u;
               m_itemQ[i][f] += m_learningRate * delta_i;

               // user and item vectors regularization: lambda * ( ||pu||^2 + ||qi||^2 )
               loss += m_lambda * puf * puf + m_lambda * qif * qif;
            }
         }
      }
      loss /= 2;
      //cout << "iter: " << it << "loss: " << loss << endl;

      if( converged( loss ) )
      {
         break;
      }
      adaptLR( it );
   }
}

void AlgFunkSvd::test( DataFrame& dataFrame )
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

double AlgFunkSvd::predict( string userId, string itemId )
{
   int userrow = m_ratingMatrix.row( userId );
   int itemcol = m_ratingMatrix.column( itemId );
   return predict( userrow, itemcol );
}

double AlgFunkSvd::predict( size_t userrow, size_t itemcol )
{
   double pred = m_globalMean;
   size_t nusers = m_ratingMatrix.users();
   size_t nitems = m_ratingMatrix.items();
   if( ( userrow < nusers ) && ( itemcol < nitems ) )
   {
      double* pu = m_userP[userrow];
      double* qi = m_itemQ[itemcol];
      pred = m_globalMean + m_userBias[userrow] + m_itemBias[itemcol] + innerProduct( pu, qi, m_nfactors );
   }
   return pred;
}

void AlgFunkSvd::reset( size_t factors, size_t maxiter, float lrate, float lambda )
{
   if( NULL != m_userP )
   {
      for( int i = 0 ; i < m_nfactors ; ++i )
      {
         delete [] m_userP[i];
      }
      delete [] m_userP;
   }

   if( NULL != m_itemQ )
   {
      for( int i = 0 ; i < m_nfactors ; ++i )
      {
         delete [] m_itemQ[i];
      }
      delete [] m_itemQ;
   }

   if( NULL != m_userBias )
   {
      delete [] m_userBias;
   }

   if( NULL != m_itemBias )
   {
      delete [] m_itemBias;
   }

   m_nfactors = factors;
   m_maxIter = maxiter;
   m_learningRate = lrate;
   m_lambda = lambda;

   size_t nusers = m_ratingMatrix.users();
   size_t nitems = m_ratingMatrix.items();

   mt19937 rng;
   normal_distribution<> nd( 0, 0.1 );
   variate_generator< boost::mt19937&, boost::normal_distribution<> > var_nor( rng, nd );

   m_userBias = new double[nusers];
   m_userP = new double*[nusers];
   for( int i = 0 ; i < nusers ; ++i )
   {
      m_userBias[i] = var_nor();
      m_userP[i] = new double[m_nfactors];
      for( int f = 0 ; f < m_nfactors ; ++f )
      {
         m_userP[i][f] = var_nor();
      }
   }

   m_itemBias = new double[nitems];
   m_itemQ = new double*[nitems];
   for( int i = 0 ; i < nitems ; ++i )
   {
      m_itemBias[i] = var_nor();
      m_itemQ[i] = new double[m_nfactors];
      for( int f = 0 ; f < m_nfactors ; ++f )
      {
         m_itemQ[i][f] = var_nor();
      }
   }
}

bool AlgFunkSvd::converged( double loss )
{
   double dloss = m_prevLoss - loss;
   bool converged = ( abs( loss ) < 1e-5 ) || ( ( dloss > 0 ) && ( dloss < 1e-5 ) );
   m_prevLoss = loss;

   return converged;
}

void AlgFunkSvd::adaptLR( double iter )
{
   if( m_learningRate > 0 )
   {
      if( m_decay > 0 && m_decay < 1 )
      {
         m_learningRate *= m_decay;
      }
   }
}

double AlgFunkSvd::innerProduct( double v1[], double v2[], size_t size )
{
   double sum = 0;
   for( int i = 0 ; i < size ; ++i )
   {
      sum += v1[i] * v2[i];
   }
   return sum;
}



