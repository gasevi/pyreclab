#include "RMSE.h"

#include <iostream>
#include <cmath>

using namespace std;

RMSE::RMSE()
: m_sqErrorAccum( 0 )
{
}

void RMSE::append( double rating, double prediction )
{
   m_rateData.push_back( pair<double, double>( rating, prediction ) );
   m_sqErrorAccum += pow( rating - prediction, 2 );
}

void RMSE::clear()
{
   m_rateData.clear();
   m_sqErrorAccum = 0;
}

double RMSE::eval()
{
   return m_sqErrorAccum == 0 ? m_sqErrorAccum : sqrt( m_sqErrorAccum / m_rateData.size() );
}


