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

   void setToInf( int idx );

   void print();

private:

   int m_dim;

   //std::vector<int> m_triangElements;
   double* m_elements;

};

#endif // __SYMM_MATRIX_H__

