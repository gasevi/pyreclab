#ifndef __MAE_H__
#define __MAE_H__

#include <vector>
#include <utility>

class MAE
{
public:

   void append( double rating, double prediction );

   void clear();

   double eval();

private:

   std::vector< std::pair<double, double> > m_rateData;

   double m_absErrorAccum;

};

#endif // __MAE_H__


