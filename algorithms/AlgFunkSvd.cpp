#include "AlgFunkSvd.h"
#include "Similarity.h"
#include "MaxHeap.h"

#include <random>

using namespace std;


AlgFunkSvd::AlgFunkSvd( DataReader& dreader,
                        int userpos,
                        int itempos,
                        int ratingpos )
: RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >( dreader, userpos, itempos, ratingpos ),
  m_nfactors( 1000 ),
  m_userP( NULL ),
  m_itemQ( NULL ),
  m_userBias( NULL ),
  m_itemBias( NULL ),
  m_maxIter( 100 ),
  m_lambda( 0.1 ),
  m_learningRate( 0.01 ),
  m_decay( -1 ),
  m_prevLoss( 0 ),
  m_currLoss( 0 )
{
   size_t nusers = m_ratingMatrix.users();
   size_t nitems = m_ratingMatrix.items();

   random_device rd;
   mt19937 randGen( rd() );
   normal_distribution<> normal_dist( 0, 0.1 );

   m_userBias = new double[nusers];
   m_userP = new double*[nusers];
   for( size_t i = 0 ; i < nusers ; ++i )
   {
      m_userBias[i] = normal_dist( randGen );
      m_userP[i] = new double[m_nfactors];
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_userP[i][f] = normal_dist( randGen );
      }
   }

   m_itemBias = new double[nitems];
   m_itemQ = new double*[nitems];
   for( size_t i = 0 ; i < nitems ; ++i )
   {
      m_itemBias[i] = normal_dist( randGen );
      m_itemQ[i] = new double[m_nfactors];
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_itemQ[i][f] = normal_dist( randGen );
      }
   }
}

AlgFunkSvd::AlgFunkSvd( size_t factors,
                        DataReader& dreader,
                        int userpos,
                        int itempos,
                        int ratingpos )
: RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >( dreader, userpos, itempos, ratingpos ),
  m_nfactors( factors ),
  m_userP( NULL ),
  m_itemQ( NULL ),
  m_userBias( NULL ),
  m_itemBias( NULL ),
  m_maxIter( 100 ),
  m_lambda( 0.1 ),
  m_learningRate( 0.01 ),
  m_decay( -1 ),
  m_prevLoss( 0 ),
  m_currLoss( 0 )
{
   size_t nusers = m_ratingMatrix.users();
   size_t nitems = m_ratingMatrix.items();

   random_device rd;
   mt19937 randGen( rd() );
   normal_distribution<> normal_dist( 0, 0.1 );

   m_userBias = new double[nusers];
   m_userP = new double*[nusers];
   for( size_t i = 0 ; i < nusers ; ++i )
   {
      m_userBias[i] = normal_dist( randGen );
      m_userP[i] = new double[m_nfactors];
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_userP[i][f] = normal_dist( randGen );
      }
   }

   m_itemBias = new double[nitems];
   m_itemQ = new double*[nitems];
   for( size_t i = 0 ; i < nitems ; ++i )
   {
      m_itemBias[i] = normal_dist( randGen );
      m_itemQ[i] = new double[m_nfactors];
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_itemQ[i][f] = normal_dist( randGen );
      }
   }
}

AlgFunkSvd::~AlgFunkSvd()
{
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

   if( NULL != m_userBias )
   {
      delete [] m_userBias;
   }

   if( NULL != m_itemBias )
   {
      delete [] m_itemBias;
   }
}

int AlgFunkSvd::train( size_t factors, size_t maxiter, float lrate, float lambda, FlowControl& fcontrol )
{
   reset( factors, maxiter, lrate, lambda );
   return train( fcontrol );
}

int AlgFunkSvd::train( size_t maxiter, float lrate, float lambda, FlowControl& fcontrol )
{
   m_maxIter = maxiter;
   m_learningRate = lrate;
   m_lambda = lambda;
   return train( fcontrol );
}

int AlgFunkSvd::train( FlowControl& fcontrol )
{
   for( size_t it = 0 ; it < m_maxIter ; ++it )
   {
      size_t nusers = m_ratingMatrix.users();
      for( size_t u = 0 ; u < nusers ; ++u )
      {
         SparseRow< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> > row = m_ratingMatrix.userVector( u );
         SparseRow< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >::iterator ind;
         SparseRow< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >::iterator end = row.end();
         for( ind = row.begin() ; ind != end ; ++ind )
         {
            int i = ind.index();

            double pred = predict( u, i );
            double eui = *ind - pred;

            // squared error: ( rui - ( mu + bu + bi + pu*qi ) )^2
            m_currLoss += eui * eui;

            double bu = m_userBias[u];
            double bi = m_itemBias[i];

            // bias regularization: lambda * ( bu^2 + bi^2 )
            m_currLoss += m_lambda * bu * bu;
            m_currLoss += m_lambda * bi * bi;

            // update biases
            double buNext = eui - m_lambda * bu;
            m_userBias[u] += m_learningRate * buNext;
            double biNext = eui - m_lambda * bi;
            m_itemBias[i] += m_learningRate * biNext;

            for( size_t f = 0 ; f < m_nfactors ; ++f )
            {
               double puf = m_userP[u][f];
               double qif = m_itemQ[i][f];

               double delta_u = eui * qif - m_lambda * puf;
               double delta_i = eui * puf - m_lambda * qif;

               // update user and item matrices
               m_userP[u][f] += m_learningRate * delta_u;
               m_itemQ[i][f] += m_learningRate * delta_i;

               // user and item vectors regularization: lambda * ( ||pu||^2 + ||qi||^2 )
               m_currLoss += m_lambda * puf * puf + m_lambda * qif * qif;

               if( fcontrol.interrupt() )
               {
                  return STOPPED;
               }
            }
         }
      }
      m_currLoss /= 2;
      //cout << "iter: " << it << "m_currLoss: " << m_currLoss << endl;

      if( converged( m_currLoss ) )
      {
         break;
      }
      adaptLR( it );
   }

   return FINISHED;
}

void AlgFunkSvd::reset()
{
   size_t nusers = m_ratingMatrix.users();
   size_t nitems = m_ratingMatrix.items();

   random_device rd;
   mt19937 randGen( rd() );
   normal_distribution<> normal_dist( 0, 0.1 );

   for( size_t i = 0 ; i < nusers ; ++i )
   {
      m_userBias[i] = normal_dist( randGen );
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_userP[i][f] = normal_dist( randGen );
      }
   }

   for( size_t i = 0 ; i < nitems ; ++i )
   {
      m_itemBias[i] = normal_dist( randGen );
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_itemQ[i][f] = normal_dist( randGen );
      }
   }

   m_currLoss = 0;
}

double AlgFunkSvd::loss()
{
   return m_currLoss;
}

double AlgFunkSvd::predict( string& userId, string& itemId )
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

   random_device rd;
   mt19937 randGen( rd() );
   normal_distribution<> normal_dist( 0, 0.1 );

   m_userBias = new double[nusers];
   m_userP = new double*[nusers];
   for( size_t i = 0 ; i < nusers ; ++i )
   {
      m_userBias[i] = normal_dist( randGen );
      m_userP[i] = new double[m_nfactors];
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_userP[i][f] = normal_dist( randGen );
      }
   }

   m_itemBias = new double[nitems];
   m_itemQ = new double*[nitems];
   for( size_t i = 0 ; i < nitems ; ++i )
   {
      m_itemBias[i] = normal_dist( randGen );
      m_itemQ[i] = new double[m_nfactors];
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_itemQ[i][f] = normal_dist( randGen );
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
   for( size_t i = 0 ; i < size ; ++i )
   {
      sum += v1[i] * v2[i];
   }
   return sum;
}



