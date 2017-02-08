#include "MAE.h"

#include <cmath>

using namespace std;

void MAE::append( double rating, double prediction )
{
   m_rateData.push_back( pair<double, double>( rating, prediction ) );
   m_absErrorAccum += fabs( rating - prediction );
}

void MAE::clear()
{
   m_rateData.clear();
   m_absErrorAccum = 0;
}

double MAE::eval()
{
   return m_absErrorAccum == 0 ? m_absErrorAccum : m_absErrorAccum / m_rateData.size();
}


