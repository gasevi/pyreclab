#ifndef __SPARSE_MATRIX_H__
#define __SPARSE_MATRIX_H__

#include "SparseRow.h"
#include "SparseColumn.h"

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <iostream>


template <class smatrix_t>
class SparseMatrix
{
public:

   SparseMatrix();

   ~SparseMatrix();

   size_t rows();

   size_t columns();

   double get( size_t row, size_t col );

   void set( size_t row, size_t col, double value );

   SparseRow<smatrix_t> row( size_t row );

   SparseColumn<smatrix_t> column( size_t column );

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

   smatrix_t* m_smatrix;

};


template <class smatrix_t>
SparseMatrix<smatrix_t>::SparseMatrix()
: m_smatrix( NULL )
{
}

template <class smatrix_t>
SparseMatrix<smatrix_t>::~SparseMatrix()
{
   if( NULL != m_smatrix )
   {
      delete m_smatrix;
   }
}

template <class smatrix_t>
size_t SparseMatrix<smatrix_t>::rows()
{
   if( NULL == m_smatrix )
   {
      return 0;
   }
   return m_smatrix->size1();
}

template <class smatrix_t>
size_t SparseMatrix<smatrix_t>::columns()
{
   if( NULL == m_smatrix )
   {
      return 0;
   }
   return m_smatrix->size2();
}

template <class smatrix_t>
double SparseMatrix<smatrix_t>::get( size_t row, size_t col )
{
   return m_smatrix->operator()( row, col );
}

template <class smatrix_t>
void SparseMatrix<smatrix_t>::set( size_t row, size_t col, double value )
{
   if( NULL == m_smatrix )
   {
      return;
   }
   if( 0 != value )
   {
      m_smatrix->operator()( row, col ) = value;
   }
}

template <class smatrix_t>
SparseRow<smatrix_t> SparseMatrix<smatrix_t>::row( size_t row )
{
   SparseRow<smatrix_t> srow( *m_smatrix, row );
   return srow;
}

template <class smatrix_t>
SparseColumn<smatrix_t> SparseMatrix<smatrix_t>::column( size_t column )
{
   SparseColumn<smatrix_t> scolumn( *m_smatrix, column );
   return scolumn;
}

template <class smatrix_t>
void SparseMatrix<smatrix_t>::resize( size_t nrows, size_t ncols )
{
   if( NULL == m_smatrix )
   {
      //std::cout << "initializing matrix with size: " << nrows << "x" << ncols << std::endl;
      m_smatrix = new smatrix_t( nrows, ncols );
   }
   else
   {
      m_smatrix->resize( nrows, ncols );
   }
}

template <class smatrix_t>
void SparseMatrix<smatrix_t>::add( size_t row, size_t col, double value )
{
   m_smatrix->operator()( row, col ) += value;
}

template <class smatrix_t>
unsigned int SparseMatrix<smatrix_t>::nonZeroRow( size_t row )
{
   unsigned int count = 0;
   boost::numeric::ublas::matrix_row< smatrix_t > mr( *m_smatrix, row );
   typename boost::numeric::ublas::matrix_row< smatrix_t >::iterator ind;
   typename boost::numeric::ublas::matrix_row< smatrix_t >::iterator end = mr.end();
   for( ind = mr.begin() ; ind != end ; ++ind )
   {
      count += 1;
   }
   return count;
}

template <class smatrix_t>
unsigned int SparseMatrix<smatrix_t>::nonZeroCol( size_t col )
{
   unsigned int count = 0;
   boost::numeric::ublas::matrix_column< smatrix_t > mc( *m_smatrix, col );
   typename boost::numeric::ublas::matrix_column< smatrix_t >::iterator ind;
   typename boost::numeric::ublas::matrix_column< smatrix_t >::iterator end = mc.end();
   for( ind = mc.begin() ; ind != end ; ++ind )
   {
      count += 1;
   }
   return count;
}

template <class smatrix_t>
double SparseMatrix<smatrix_t>::meanRow( size_t row )
{
   double sum = 0;
   unsigned int count = 0;
   boost::numeric::ublas::matrix_row< smatrix_t > mr( *m_smatrix, row );
   typename boost::numeric::ublas::matrix_row< smatrix_t >::iterator ind;
   typename boost::numeric::ublas::matrix_row< smatrix_t >::iterator end = mr.end();
   for( ind = mr.begin() ; ind != end ; ++ind )
   {
      sum += *ind;
      count += 1;
   }
   return count != 0 ? sum/count : 0;
}

template <class smatrix_t>
double SparseMatrix<smatrix_t>::meanColumn( size_t col )
{
   double sum = 0;
   unsigned int count = 0;
   boost::numeric::ublas::matrix_column< smatrix_t > mc( *m_smatrix, col );
   typename boost::numeric::ublas::matrix_column< smatrix_t >::iterator ind;
   typename boost::numeric::ublas::matrix_column< smatrix_t >::iterator end = mc.end();
   for( ind = mc.begin() ; ind != end ; ++ind )
   {
      sum += *ind;
      count += 1;
   }
   return count != 0 ? sum/count : 0;
}

template <class smatrix_t>
int SparseMatrix<smatrix_t>::sumRow( int row, double& sum )
{
   unsigned int count = 0;
   sum = 0;
   boost::numeric::ublas::matrix_row< smatrix_t > mr( *m_smatrix, row );
   typename boost::numeric::ublas::matrix_row< smatrix_t >::iterator ind;
   typename boost::numeric::ublas::matrix_row< smatrix_t >::iterator end = mr.end();
   for( ind = mr.begin() ; ind != end ; ++ind )
   {
      sum += *ind;
      count += 1;
   }
   return count;
}

template <class smatrix_t>
int SparseMatrix<smatrix_t>::sumColumn( int col, double& sum )
{
   unsigned int count = 0;
   sum = 0;
   boost::numeric::ublas::matrix_column< smatrix_t > mc( *m_smatrix, col );
   typename boost::numeric::ublas::matrix_column< smatrix_t >::iterator ind;
   typename boost::numeric::ublas::matrix_column< smatrix_t >::iterator end = mc.end();
   for( ind = mc.begin() ; ind != end ; ++ind )
   {
      sum += *ind;
      count += 1;
   }
   return count;
}

template <class smatrix_t>
int SparseMatrix<smatrix_t>::sumDiffColumns( size_t i, size_t j, double& sumdiff )
{
   unsigned int count = 0;
   sumdiff = 0;
   boost::numeric::ublas::matrix_column< smatrix_t > mcdiff( *m_smatrix, i );
   boost::numeric::ublas::matrix_column< smatrix_t > mcj( *m_smatrix, j );
   mcdiff -= mcj;
   typename boost::numeric::ublas::matrix_column< smatrix_t >::iterator ind;
   typename boost::numeric::ublas::matrix_column< smatrix_t >::iterator end = mcdiff.end();
   for( ind = mcdiff.begin() ; ind != end ; ++ind )
   {
      sumdiff += *ind;
      count += 1;
   }
   return count;
}

template <class smatrix_t>
void SparseMatrix<smatrix_t>::print()
{
   std::cout << "matrix: " << *m_smatrix << std::endl;
}


#endif // __SPARSE_MATRIX_H__


