#ifndef __RATING_MATRIX_H__
#define __RATING_MATRIX_H__

#include "DataReader.h"
#include "SparseMatrix.h"
#include "SparseRow.h"

#include <string>


class RatingMatrix
{
public:

   RatingMatrix();

   RatingMatrix( DataReader& dreader, int userpos = 1, int itempos = 2, int ratingpos = 3 );

   size_t items();

   size_t users();

   double get( size_t row, size_t col );

   std::string userId( size_t row );

   std::string itemId( size_t col );

   int row( std::string userId );

   int column( std::string itemId );

   SparseRow userVector( size_t row );

   SparseRow userVector( std::string userId );

   SparseColumn itemVector( size_t col );

   SparseColumn itemVector( std::string itemId );

   size_t numRatings();

   double sumRatings();

   int nonZeroRow( size_t row );

   int nonZeroCol( size_t col );

   int sumRow( size_t row, double& sum );

   int sumColumn( size_t col, double& sum );

   double meanRow( size_t row );

   double meanColumn( size_t col );

   int sumDiffColumns( size_t i, size_t j, double& sumdiff );

private:

   std::map<std::string,size_t> m_userMapper;

   std::map<std::string,size_t> m_itemMapper;

   SparseMatrix m_smatrix;

   size_t m_numRatings;

   double m_sumRatings;

   double m_minRating;

   double m_maxRating;

};

#endif // __RATING_MATRIX_H__

