#ifndef __ALG_USER_AVERAGE_H__
#define __ALG_USER_AVERAGE_H__

#include "RecSysAlgorithm.h"
#include "FlowControl.h"

#include <string>
#include <map>


class AlgUserAvg
      : public RecSysAlgorithm< boost::numeric::ublas::mapped_matrix<double, boost::numeric::ublas::row_major> >
{
public:

   AlgUserAvg( DataReader& dreader,
               int userpos = 0,
               int itempos = 1,
               int ratingpos = 2 );

   ~AlgUserAvg();

   int train( FlowControl& fcontrol );

   double predict( std::string& userId, std::string& itemId );

   double predict( size_t userrow, size_t itemcol );
 
private:

   double* m_meanRatingByUserRow;

};

#endif // __ALG_USER_AVERAGE_H__

