#ifndef __ALG_SLOPE_ONE_H__
#define __ALG_SLOPE_ONE_H__

#include "RecSysAlgorithm.h"

#include <string>


class AlgSlopeOne
      : public RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >
{
public:

   AlgSlopeOne( DataReader& dreader,
                int userpos = 0,
                int itempos = 1,
                int ratingpos = 2 );

   int train();

   void test( DataFrame& dataFrame );

   double predict( std::string userId, std::string itemId );
 
private:

   SparseMatrix< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> > m_devMatrix;

   SparseMatrix< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> > m_cardMatrix;

};

#endif // __ALG_SLOPE_ONE_H__

