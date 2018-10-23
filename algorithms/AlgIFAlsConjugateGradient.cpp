#include "AlgIFAlsConjugateGradient.h"
#include "Similarity.h"
#include "MaxHeap.h"
#include "RecSysAlgorithm.h"

#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/banded.hpp>
#include <boost/numeric/ublas/lu.hpp>

#include <cstdlib>
#include <sys/time.h>

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;


AlgIFAlsConjugateGradient::AlgIFAlsConjugateGradient( DataReader& dreader,
                                                      int userpos,
                                                      int itempos,
                                                      int obspos )
: m_running( true ),
  m_nfactors( 100 ),
  m_alsNumIter( 5 ),
  m_lambda( 10 ),
  m_alpha( 40 ),
  m_cgNumIter( 2 ),
  m_pVecPu( NULL ),
  m_pVecPi( NULL ),
  m_pMatCu( NULL ),
  m_pMatCi( NULL )
{
   map< pair<size_t, size_t>, double > inputData;
   while( !dreader.eof() )
   {
      std::vector<string> line;
      dreader.readline( line );
      if( line.empty() )
      {
         break;
      }
      std::string userId = line[userpos];
      std::string itemId = line[itempos];

      double rui = 0;
      std::stringstream ss;
      ss << line[obspos];
      ss >> rui;

      //std::cout << "user [" << userId << "] item [" << itemId << "] rating: [" << rating << "]" << std::endl;
      size_t row, col;
      std::map<std::string,size_t>::iterator ind;

      ind = m_fUserMapper.find( userId );
      if( ind == m_fUserMapper.end() )
      {
         row = m_fUserMapper.size();
         m_fUserMapper[userId] = row;
         m_bUserMapper[row] = userId;
      }
      else
      {
         row = ind->second;
      }

      ind = m_fItemMapper.find( itemId );
      if( ind == m_fItemMapper.end() )
      {
         col = m_fItemMapper.size();
         m_fItemMapper[itemId] = col;
         m_bItemMapper[col] = itemId;
      }
      else
      {
         col = ind->second;
      }

      if( inputData.find( std::pair<size_t,size_t>( row, col ) ) != inputData.end() )
      {
         cout << "warning: <user: " << userId
              << " ,item:" << itemId
              << "> pair duplicated in " << dreader.currentline()
              << endl;
         continue;
      }
      inputData[pair<size_t,size_t>( row, col )] = rui;
   }

   m_pVecPu = new mapped_vector<double>*[m_fUserMapper.size()];
   for( size_t u = 0 ; u < m_fUserMapper.size() ; u++ )
   {
      m_pVecPu[u] = new mapped_vector<double>( m_fItemMapper.size() );
   }

   m_pMatCu = new diagonal_matrix<double, row_major>*[m_fUserMapper.size()];
   for( size_t u = 0 ; u < m_fUserMapper.size() ; u++ )
   {
      m_pMatCu[u] = new diagonal_matrix<double, row_major>( m_fItemMapper.size(), m_fItemMapper.size() );
   }

   m_pVecPi = new mapped_vector<double>*[m_fItemMapper.size()];
   for( size_t i = 0 ; i < m_fItemMapper.size() ; i++ )
   {
      m_pVecPi[i] = new mapped_vector<double>( m_fUserMapper.size() );
   }

   m_pMatCi = new diagonal_matrix<double, row_major>*[m_fItemMapper.size()];
   for( size_t i = 0 ; i < m_fItemMapper.size() ; i++ )
   {
      m_pMatCi[i] = new diagonal_matrix<double, row_major>( m_fUserMapper.size(), m_fUserMapper.size() );
   }

   map< pair<size_t, size_t>, double >::iterator ind;
   map< pair<size_t, size_t>, double >::iterator end = inputData.end();
   for( ind = inputData.begin() ; ind != end ; ++ind )
   {
      size_t row = ind->first.first;
      size_t col = ind->first.second;
      double observation = ind->second;

      m_pVecPu[row]->operator()( col ) = 1;
      m_pMatCu[row]->operator()( col, col ) = 1 + m_alpha*observation;

      m_pVecPi[col]->operator()( row ) = 1;
      m_pMatCi[col]->operator()( row, row ) = 1 + m_alpha*observation;
   }
}

AlgIFAlsConjugateGradient::~AlgIFAlsConjugateGradient()
{
   m_Xu.clear();
   m_Yi.clear();

   if( NULL != m_pVecPu )
   {
      size_t nusers = m_fUserMapper.size();
      for( size_t u = 0 ; u < nusers ; ++u )
      {
         delete m_pVecPu[u];
      }
      delete [] m_pVecPu;
   }

   if( NULL != m_pMatCu )
   {
      size_t nusers = m_fUserMapper.size();
      for( size_t u = 0 ; u < nusers ; ++u )
      {
         delete m_pMatCu[u];
      }
      delete [] m_pMatCu;
   }

   if( NULL != m_pVecPi )
   {
      size_t nitems = m_fItemMapper.size();
      for( size_t i = 0 ; i < nitems ; ++i )
      {
         delete m_pVecPi[i];
      }
      delete [] m_pVecPi;
   }

   if( NULL != m_pMatCi )
   {
      size_t nitems = m_fItemMapper.size();
      for( size_t i = 0 ; i < nitems ; ++i )
      {
         delete m_pMatCi[i];
      }
      delete [] m_pMatCi;
   }
}

int AlgIFAlsConjugateGradient::train( size_t factors, size_t alsNumIter, float lambda, size_t cgNumIter, FlowControl& fcontrol )
{
   reset( factors, alsNumIter, lambda, cgNumIter );
   return train( fcontrol );
}

int AlgIFAlsConjugateGradient::train( FlowControl& fcontrol )
throw( runtime_error& )
{
   size_t nusers = m_fUserMapper.size();
   size_t nitems = m_fItemMapper.size();
   identity_matrix<double> Iu( nusers );
   identity_matrix<double> Ii( nitems );

   for( size_t i = 0 ; i < m_alsNumIter ; ++i )
   {
      // User stage
      conjugateGradient( m_Yi, m_Xu, m_pMatCu, m_cgNumIter );
      if( fcontrol.interrupt() )
      {
         return STOPPED;
      }

      // Item stage
      conjugateGradient( m_Xu, m_Yi, m_pMatCi, m_cgNumIter );
      if( fcontrol.interrupt() )
      {
         return STOPPED;
      }
   }

   return FINISHED;
}

void AlgIFAlsConjugateGradient::conjugateGradient( matrix<double>& Y,
                                                   matrix<double>& X,
                                                   diagonal_matrix<double, row_major>** Cu,
                                                   size_t numIter )
{
   identity_matrix<double> If( m_nfactors );
   matrix<double> Yt = trans( Y );
   matrix<double> YtYlambdaI = prod( Yt, Y ) + If*m_lambda;

   size_t numElements = X.size1();
   for( size_t i = 0 ; i < numElements ; ++i )
  {
      matrix_row< matrix<double> > Xi( X, i );
      boost::numeric::ublas::vector<double> R = -prod( YtYlambdaI, Xi );

      diagonal_matrix<double>::iterator1 ind1;
      diagonal_matrix<double>::iterator1 end1 = Cu[i]->end1();
      for( ind1 = Cu[i]->begin1() ; ind1 != end1 ; ++ind1 )
      {
         diagonal_matrix<double>::iterator2 ind2;
         diagonal_matrix<double>::iterator2 end2 = ind1.end();
         for( ind2 = ind1.begin() ; ind2 != end2 ; ++ind2 )
         {
            double cui = *ind2;
            matrix_row< matrix<double> > Yi( Y, ind1.index1() );
            double yixi = inner_prod( Yi, Xi );
            R += ( cui - ( cui - 1 ) * yixi ) * Yi;
         }
      }

      boost::numeric::ublas::vector<double> P = R;
      double rtrOld = inner_prod( R, R );

      // Conjugate gradient body
      for( size_t j = 0 ; j < numIter ; ++j )
      {
         boost::numeric::ublas::vector<double> AP = prod( YtYlambdaI, P );
         diagonal_matrix<double>::iterator1 ind1;
         diagonal_matrix<double>::iterator1 end1 = Cu[i]->end1();
         for( ind1 = Cu[i]->begin1() ; ind1 != end1 ; ++ind1 )
         {  
            diagonal_matrix<double>::iterator2 ind2;
            diagonal_matrix<double>::iterator2 end2 = ind1.end();
            for( ind2 = ind1.begin() ; ind2 != end2 ; ++ind2 )
            {  
               double cui = *ind2;
               matrix_row< matrix<double> > Yi( Y, ind1.index1() );
               double yiP = inner_prod( Yi, P );
               AP += ( ( cui - 1 ) * yiP ) * Yi;
            }
         }

         double alpha = rtrOld / inner_prod( P, AP );
         Xi += alpha * P;
         R -= alpha * AP;
         double rtrNew = inner_prod( R, R );
         P = R + ( rtrNew/rtrOld ) * P;
         rtrOld = rtrNew;

         if( !m_running )
         {
            return;
         }

      }
   }
}

void AlgIFAlsConjugateGradient::conjugateGradientSlow( matrix<double>& Y,
                                                       matrix<double>& X,
                                                       diagonal_matrix<double, row_major>** Cu,
                                                       boost::numeric::ublas::mapped_vector<double>** Pu,
                                                       identity_matrix<double>& Iu,
                                                       size_t numIter )
{
   identity_matrix<double> If( m_nfactors );
   matrix<double> Yt = trans( Y );
   matrix<double> YtYlambdaI = prod( Yt, Y ) + If*m_lambda;

   size_t numElements = X.size1();
   for( size_t i = 0 ; i < numElements ; ++i )
   {
      boost::numeric::ublas::matrix<double> subprod1 = prod( Yt, *(Cu[i]) );
      boost::numeric::ublas::vector<double> b = prod( subprod1, *(Pu[i]) );

      boost::numeric::ublas::matrix<double> subprod2 = prod( ( *(Cu[i]) - Iu ), Y );
      boost::numeric::ublas::matrix<double> A = YtYlambdaI + prod( Yt, subprod2 );

      matrix_row< matrix<double> > Xi( X, i );
      boost::numeric::ublas::vector<double> R = b - prod( A, Xi );
      boost::numeric::ublas::vector<double> P = R;

      double rtrOld = inner_prod( R, R );

      // Conjugate gradient body
      for( size_t j = 0 ; j < numIter ; ++j )
      {
         boost::numeric::ublas::vector<double> AP = prod( A, P );

         double alpha = rtrOld / inner_prod( P, AP );
         Xi += alpha * P;
         R -= alpha * AP;
         double rtrNew = inner_prod( R, R );
         P = R + ( rtrNew/rtrOld ) * P;
         rtrOld = rtrNew;
      }
   }
}

double AlgIFAlsConjugateGradient::predict( size_t userrow, size_t itemcol )
{
   double pred = 0;
   matrix_row< matrix<double> > xu( m_Xu, userrow );
   matrix_row< matrix<double> > yi( m_Yi, itemcol );

   matrix_row< matrix<double> >::iterator ind;
   matrix_row< matrix<double> >::iterator end = xu.end();
   for( ind = xu.begin() ; ind != end ; ++ind )
   {
      pred += (*ind) * yi( ind.index() );
   }

   return pred;
}

bool AlgIFAlsConjugateGradient::recommend( const std::string& userId,
                                           size_t n,
                                           std::vector<std::string>& ranking,
                                           bool includeRated )
throw( invalid_argument& )
{
   if( m_fUserMapper.find( userId ) == m_fUserMapper.end() )
   {  
      std::ostringstream eMsg;
      eMsg << "user id '" << userId << "' was not included in training set";
      throw invalid_argument( eMsg.str() );
   }
   int userrow = m_fUserMapper[userId];

   MaxHeap maxheap;
   for( size_t itemcol = 0 ; itemcol < m_fItemMapper.size() ; ++itemcol )
   {
      if( includeRated || ( 0 == m_pVecPu[userrow]->operator()( itemcol ) ) )
      {
         double score = predict( userrow, itemcol );
         std::pair<double, size_t> e = std::pair<double, size_t>( score, itemcol );
         maxheap.push( e );
      }
   }

   if( maxheap.empty() )
   {
      return false;
   }

   for( size_t i = 0 ; i < n ; ++i )
   {
      std::pair<double, size_t> e = maxheap.pop();
      ranking.push_back( m_bItemMapper[e.second] );
   }

   return true;
}

void AlgIFAlsConjugateGradient::reset( size_t factors, size_t alsNumIter, float lambda, size_t cgNumIter )
{
   m_Xu.clear();
   m_Yi.clear();

   m_nfactors = factors;
   m_alsNumIter = alsNumIter;
   m_lambda = lambda;
   m_cgNumIter = cgNumIter;

   size_t nusers = m_fUserMapper.size();
   size_t nitems = m_fItemMapper.size();

   mt19937 rng;
   normal_distribution<> nd( 0, 0.1 );
   variate_generator< mt19937&, normal_distribution<> > var_nor( rng, nd );

   m_Xu.resize( nusers, m_nfactors );
   for( size_t u = 0 ; u < nusers ; ++u )
   {
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_Xu( u, f ) = var_nor();
      }
   }

   m_Yi.resize( nitems, m_nfactors );
   for( size_t i = 0 ; i < nitems ; ++i )
   {
      for( size_t f = 0 ; f < m_nfactors ; ++f )
      {
         m_Yi( i, f ) = var_nor();
      }
   }
}


