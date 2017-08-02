#ifndef __DENSE_MATRIX_H__
#define __DENSE_MATRIX_H__

#include <cstddef>

class DenseMatrix
{
public:

   DenseMatrix( size_t rows, size_t columns );

   DenseMatrix( DenseMatrix& matrix );

   ~DenseMatrix();

   void set( size_t row, size_t col, double element );

   double get( size_t row, size_t col );

   void add( size_t row, size_t col, double element );

   size_t rows();

   size_t columns();

   void print();

private:

   size_t m_rows;

   size_t m_columns;

   double* m_elements;

};

#endif // __DENSE_MATRIX_H__

