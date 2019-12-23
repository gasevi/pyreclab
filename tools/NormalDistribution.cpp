#include "NormalDistribution.h"

#include <ctime>

using namespace std;


NormalDistribution::NormalDistribution( float mean, float std )
: m_nd( mean, std ),
  m_var_nor( m_rng, m_nd )
{
   m_rng.seed( static_cast<unsigned int>( time( 0 ) ) );
}

double NormalDistribution::operator()()
{
   return m_var_nor();
}


