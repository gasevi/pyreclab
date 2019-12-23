#include "UniformRealDistribution.h"

#include <ctime>

using namespace std;


UniformRealDistribution::UniformRealDistribution( float min, float max )
: m_urd( min, max ),
  m_var_uni( m_rng, m_urd )
{
   m_rng.seed( static_cast<uint32_t>( clock() ) );
}

double UniformRealDistribution::operator()()
{
   return m_var_uni();
}

