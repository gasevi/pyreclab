#ifndef __RMSE_H__
#define __RMSE_H__

#include <vector>
#include <utility>

class RMSE
{
public:

   RMSE();

   void append( double rating, double prediction );

   void clear();

   double eval();

private:

   std::vector< std::pair<double, double> > m_rateData;

   double m_sqErrorAccum;

};

#endif // __RMSE_H__


