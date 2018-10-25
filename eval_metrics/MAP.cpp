#include "MAP.h"

#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

void MAP::append( std::vector<std::string>& retrieved, std::vector<std::string>& relevant )
{
   float k = 1;
   float tp = 0;
   double avgPrecision = 0;
   vector<string>::iterator ind;
   vector<string>::iterator end = retrieved.end();
   for( ind = retrieved.begin() ; ind != end ; ++ind )
   {
      if( find( relevant.begin(), relevant.end(), *ind ) != relevant.end() )
      {
         ++tp;
         avgPrecision += tp / k;
      }
      ++k;
   }
   avgPrecision = tp > 0 ? avgPrecision / tp : 0;
   m_avgPrecision.push_back( avgPrecision );
}

void MAP::clear()
{
   m_avgPrecision.clear();
}

double MAP::eval()
{
   return m_avgPrecision.size() > 0 ? std::accumulate( m_avgPrecision.begin(), m_avgPrecision.end(), 0.0 ) / m_avgPrecision.size() : 0;
}


