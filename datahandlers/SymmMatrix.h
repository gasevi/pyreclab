#ifndef __SYMM_MATRIX_H__
#define __SYMM_MATRIX_H__

#include <cstddef>

class SymmMatrix
{
public:

   SymmMatrix( size_t side );

   SymmMatrix( SymmMatrix& distMatrix );

   ~SymmMatrix();

   void set( size_t row, size_t col, double element );

   double get( size_t row, size_t col );

   int side();

   void setToInf( size_t idx );

   void print();

private:

   size_t m_dim;

   double* m_elements;

};

#endif // __SYMM_MATRIX_H__

