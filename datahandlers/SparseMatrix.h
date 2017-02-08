#ifndef __SPARSE_MATRIX_H__
#define __SPARSE_MATRIX_H__

#include "SparseRow.h"
#include "SparseColumn.h"

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <vector>


class SparseMatrix
{
public:

   typedef boost::numeric::ublas::mapped_matrix<double> sparse_matrix_t;

   SparseMatrix();

   ~SparseMatrix();

   size_t rows();

   size_t columns();

   double get( size_t row, size_t col );

   void set( size_t row, size_t col, double value );

   SparseRow row( size_t row );

   SparseColumn column( size_t column );

   void resize( size_t row, size_t col );

   void add( size_t row, size_t col, double value );

   unsigned int nonZeroRow( size_t row );

   unsigned int nonZeroCol( size_t col );

   int sumRow( int row, double& sum );

   int sumColumn( int col, double& sum );

   double meanRow( size_t row );

   double meanColumn( size_t col );

   int sumDiffColumns( size_t i, size_t j, double& sumdiff );

   void print();

private:

   sparse_matrix_t* m_smatrix;

};

#endif // __SPARSE_MATRIX_H__

