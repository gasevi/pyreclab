#include "AlgIFAls.h"
#include "Similarity.h"
#include "MaxHeap.h"
#include "RecSysAlgorithm.h"

#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>

#include <cstdlib>
#include <sys/time.h>

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;


AlgIFAls::AlgIFAls( DataReader& dreader,
                    int userpos,
                    int itempos,
                    int obspos )
: m_running( true ),
  m_nfactors( 100 ),
  m_maxIter( 10 ),
  m_alpha( 40 ),
  m_lambda( 10 ),
  m_pVecPu( NULL ),
  m_pMatCu( NULL )
{
   map< pair<size_t, size_t>, double > inputData;
   while( !dreader.eof() )
   {
      std::vector<std::string> line = dreader.readline();
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
         std::cout << "warning: <user:" << userId << " ,item:" << itemId << "> pair duplicated" << std::endl;
         continue;
      }
      inputData[pair<size_t,size_t>( row, col )] = rui;
   }

   m_pVecPu = new mapped_vector<double>*[m_fUserMapper.size()];
   for( size_t u = 0 ; u < m_fUserMapper.size() ; u++ )
   {
      m_pVecPu[u] = new mapped_vector<double>( m_fItemMapper.size() );
   }

   m_pMatCu = new mapped_matrix<double, row_major>*[m_fUserMapper.size()];
   for( size_t u = 0 ; u < m_fUserMapper.size() ; u++ )
   {
      m_pMatCu[u] = new mapped_matrix<double, row_major>( m_fItemMapper.size(), m_fItemMapper.size() );
   }

   m_pVecPi = new mapped_vector<double>*[m_fItemMapper.size()];
   for( size_t i = 0 ; i < m_fItemMapper.size() ; i++ )
   {
      m_pVecPi[i] = new mapped_vector<double>( m_fUserMapper.size() );
   }

   m_pMatCi = new mapped_matrix<double, row_major>*[m_fItemMapper.size()];
   for( size_t i = 0 ; i < m_fItemMapper.size() ; i++ )
   {
      m_pMatCi[i] = new mapped_matrix<double, row_major>( m_fUserMapper.size(), m_fUserMapper.size() );
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

AlgIFAls::~AlgIFAls()
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

int AlgIFAls::train( size_t factors, size_t maxiter, float lambda )
{
   reset( factors, maxiter, lambda );
   return train();
}

int AlgIFAls::train()
throw( runtime_error& )
{
   size_t nusers = m_fUserMapper.size();
   size_t nitems = m_fItemMapper.size();
   identity_matrix<double> Iu( nusers );
   identity_matrix<double> Ii( nitems );
   identity_matrix<double> If( m_nfactors );

   for( size_t it = 0 ; it < m_maxIter ; ++it )
   {
      // User stage
      matrix<double> Yt = trans( m_Yi );
      matrix<double> YtYlambdaI = prod( Yt, m_Yi ) + If*m_lambda;
      for( size_t u = 0 ; u < nusers ; ++u )
      {
/*
         struct timeval tv1;
         struct timeval tv2;
         gettimeofday( &tv1, NULL );
         cout << "user " << u << endl;
*/
         matrix<double> subterm;

         // Begin: Cu - I
         mapped_matrix<double> CuminI( nitems, nitems );
         {
         mapped_matrix<double>::iterator1 ind1;
         mapped_matrix<double>::iterator1 end1 = m_pMatCu[u]->end1();
         for( ind1 = m_pMatCu[u]->begin1() ; ind1 != end1 ; ++ind1 )
         {
            mapped_matrix<double>::iterator2 ind2;
            mapped_matrix<double>::iterator2 end2 = m_pMatCu[u]->end2();
            for( ind2 = m_pMatCu[u]->begin2() ; ind2 != end2 ; ++ind2 )
            {
               CuminI( ind2.index1(), ind2.index2() ) = *ind2 - 1;
            }
         }
         }
         // End: Cu - I
         subterm = prod( Yt, CuminI );

         subterm = prod( subterm, m_Yi ) + YtYlambdaI;
         matrix<double> invYtCuYlI( m_nfactors, m_nfactors );
         bool inverted = invert( subterm, invYtCuYlI );
         if( !inverted )
         {
            throw runtime_error( "Matrix could not be inverted" );
         }
         subterm = prod( invYtCuYlI, Yt );

         // Begin: Cu * Pu
         mapped_vector<double> CuPu( nitems );
         {
         mapped_matrix<double>::iterator1 ind1;
         mapped_matrix<double>::iterator1 end1 = m_pMatCu[u]->end1();
         for( ind1 = m_pMatCu[u]->begin1() ; ind1 != end1 ; ++ind1 )
         {
            mapped_matrix<double>::iterator2 ind2;
            mapped_matrix<double>::iterator2 end2 = m_pMatCu[u]->end2();
            for( ind2 = m_pMatCu[u]->begin2() ; ind2 != end2 ; ++ind2 )
            {
               CuPu( ind2.index1() ) = *ind2;
            }
         }
         }
         // End: Cu * Pu

         matrix_row< matrix<double> > xu( m_Xu, u );
         xu = prod( subterm, CuPu );

         if( !m_running )
         {
            return STOPPED;
         }

/*
         gettimeofday( &tv2, NULL );
         float dt = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
         cout << "elapsed time: " << dt << endl;
*/
      }

      // Item stage
      matrix<double> Xt = trans( m_Xu );
      matrix<double> XtXlambdaI = prod( Xt, m_Xu ) + If*m_lambda;
      for( size_t i = 0 ; i < nitems ; ++i )
      {
/*
         struct timeval tv1;
         struct timeval tv2;
         gettimeofday( &tv1, NULL );
         cout << "item " << i << endl;
*/

         matrix<double> subterm;

         // Begin: Ci - I
         mapped_matrix<double, boost::numeric::ublas::row_major> CiminI( nusers, nusers );
         {
         mapped_matrix<double>::iterator1 ind1;
         mapped_matrix<double>::iterator1 end1 = m_pMatCi[i]->end1();
         for( ind1 = m_pMatCi[i]->begin1() ; ind1 != end1 ; ++ind1 )
         {
            mapped_matrix<double>::iterator2 ind2;
            mapped_matrix<double>::iterator2 end2 = m_pMatCi[i]->end2();
            for( ind2 = m_pMatCi[i]->begin2() ; ind2 != end2 ; ++ind2 )
            {
               CiminI( ind2.index1(), ind2.index2() ) = *ind2 - 1;
            }
         }
         }
         // End: Ci - I
         subterm = prod( Xt, CiminI );

         subterm = prod( subterm, m_Xu ) + XtXlambdaI;
         matrix<double> invXtCiXlI( m_nfactors, m_nfactors );
         bool inverted = invert( subterm, invXtCiXlI );
         if( !inverted )
         {
            throw runtime_error( "Matrix could not be inverted" );
         }
         subterm = prod( invXtCiXlI, Xt );

         // Begin: Ci * Pi
         mapped_vector<double> CiPi( nusers );
         {
         mapped_matrix<double>::iterator1 ind1;
         mapped_matrix<double>::iterator1 end1 = m_pMatCi[i]->end1();
         for( ind1 = m_pMatCi[i]->begin1() ; ind1 != end1 ; ++ind1 )
         {
            mapped_matrix<double>::iterator2 ind2;
            mapped_matrix<double>::iterator2 end2 = m_pMatCi[i]->end2();
            for( ind2 = m_pMatCi[i]->begin2() ; ind2 != end2 ; ++ind2 )
            {
               CiPi( ind2.index1() ) = *ind2;
            }
         }
         }
         // End: Ci * Pi

         matrix_row< matrix<double> > yi( m_Yi, i );
         yi = prod( subterm, CiPi );

         if( !m_running )
         {
            return STOPPED;
         }

/*
         gettimeofday( &tv2, NULL );
         float dt = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
         cout << "elapsed time: " << dt << endl;
*/
      }
   }

   return FINISHED;
}

double AlgIFAls::predict( size_t userrow, size_t itemcol )
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

bool AlgIFAls::recommend( const std::string& userId,
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

void AlgIFAls::reset( size_t factors, size_t maxiter, float lambda )
{
   m_Xu.clear();
   m_Yi.clear();

   m_nfactors = factors;
   m_maxIter = maxiter;
   m_lambda = lambda;

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

bool AlgIFAls::invert( const matrix<double>& input, matrix<double>& inverse )
{
   typedef permutation_matrix<std::size_t> pmatrix;
   // create a working copy of the input
   matrix<double> A( input );
   // create a permutation matrix for the LU-factorization
   pmatrix pm( A.size1() );
   // perform LU-factorization
   int res = lu_factorize( A, pm );
   if( res != 0 )
   {
      return false;
   }
   // create identity matrix of "inverse"
   inverse.assign( identity_matrix<double>( A.size1() ) );
   // backsubstitute to get the inverse
   lu_substitute( A, pm, inverse );
   return true;
}

