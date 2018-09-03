#include "NDCG.h"

#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

NDCG::NDCG()
: m_numQueries( 0 ),
  m_cumNDCG( 0 )
{
}

void NDCG::append( std::vector<std::string>& retrieved, std::vector<std::string>& relevant )
{
   float i = 1;
   double dcg = 0;
   double idcg = 0;

   if( !relevant.size() )
   {
      return;
   }

   vector<string>::iterator ind;
   vector<string>::iterator end = retrieved.end();
   for( ind = retrieved.begin() ; ind != end ; ++ind )
   {
      double log2ip1 = log2( i + 1 );

      if( find( relevant.begin(), relevant.end(), *ind ) != relevant.end() )
      {
         dcg += 1 / log2ip1;
      }

      if( i <= relevant.size() )
      {
         idcg += 1 / log2ip1;
      }

      ++i;
   }

   m_cumNDCG += dcg / idcg;
   ++m_numQueries;
}

void NDCG::clear()
{
   m_numQueries = 0;
   m_cumNDCG = 0;
}

double NDCG::eval()
{
   return m_numQueries > 0 ? m_cumNDCG / m_numQueries : 0;
}


