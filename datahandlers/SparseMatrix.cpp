#include "SparseMatrix.h"

#include <iostream>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace std;
using namespace boost::numeric::ublas;


SparseMatrix::SparseMatrix()
: m_smatrix( NULL )
{
}

SparseMatrix::~SparseMatrix()
{
   if( NULL != m_smatrix )
   {
      delete m_smatrix;
   }
}

size_t SparseMatrix::rows()
{
   if( NULL == m_smatrix )
   {
      return 0;
   }
   return m_smatrix->size1();
}

size_t SparseMatrix::columns()
{
   if( NULL == m_smatrix )
   {
      return 0;
   }
   return m_smatrix->size2();
}

double SparseMatrix::get( size_t row, size_t col )
{
   return m_smatrix->operator()( row, col );
}

void SparseMatrix::set( size_t row, size_t col, double value )
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

SparseRow SparseMatrix::row( size_t row )
{
   SparseRow srow( *m_smatrix, row );
   return srow;
}

SparseColumn SparseMatrix::column( size_t column )
{
   SparseColumn scolumn( *m_smatrix, column );
   return scolumn;
}

void SparseMatrix::resize( size_t nrows, size_t ncols )
{
   if( NULL == m_smatrix )
   {
      //cout << "initializing matrix with size: " << nrows << "x" << ncols << endl;
      m_smatrix = new sparse_matrix_t( nrows, ncols );
   }
   else
   {
      m_smatrix->resize( nrows, ncols );
   }
}

void SparseMatrix::add( size_t row, size_t col, double value )
{
   m_smatrix->operator()( row, col ) += value;
}

unsigned int SparseMatrix::nonZeroRow( size_t row )
{
   unsigned int count = 0;
   matrix_row< sparse_matrix_t > mr( *m_smatrix, row );
   matrix_row< sparse_matrix_t >::iterator ind;
   matrix_row< sparse_matrix_t >::iterator end = mr.end();
   for( ind = mr.begin() ; ind != end ; ++ind )
   {
      count += 1;
   }
   return count;
}

unsigned int SparseMatrix::nonZeroCol( size_t col )
{
   unsigned int count = 0;
   matrix_column< sparse_matrix_t > mc( *m_smatrix, col );
   matrix_column< sparse_matrix_t >::iterator ind;
   matrix_column< sparse_matrix_t >::iterator end = mc.end();
   for( ind = mc.begin() ; ind != end ; ++ind )
   {
      count += 1;
   }
   return count;
}

double SparseMatrix::meanRow( size_t row )
{
   double sum = 0;
   unsigned int count = 0;
   matrix_row< sparse_matrix_t > mr( *m_smatrix, row );
   matrix_row< sparse_matrix_t >::iterator ind;
   matrix_row< sparse_matrix_t >::iterator end = mr.end();
   for( ind = mr.begin() ; ind != end ; ++ind )
   {
      sum += *ind;
      count += 1;
   }
   return count != 0 ? sum/count : 0;
}

double SparseMatrix::meanColumn( size_t col )
{
   double sum = 0;
   unsigned int count = 0;
   matrix_column< sparse_matrix_t > mc( *m_smatrix, col );
   matrix_column< sparse_matrix_t >::iterator ind;
   matrix_column< sparse_matrix_t >::iterator end = mc.end();
   for( ind = mc.begin() ; ind != end ; ++ind )
   {
      sum += *ind;
      count += 1;
   }
   return count != 0 ? sum/count : 0;
}

int SparseMatrix::sumRow( int row, double& sum )
{
   unsigned int count = 0;
   sum = 0;
   matrix_row< sparse_matrix_t > mr( *m_smatrix, row );
   matrix_row< sparse_matrix_t >::iterator ind;
   matrix_row< sparse_matrix_t >::iterator end = mr.end();
   for( ind = mr.begin() ; ind != end ; ++ind )
   {
      sum += *ind;
      count += 1;
   }
   return count;
}

int SparseMatrix::sumColumn( int col, double& sum )
{
   unsigned int count = 0;
   sum = 0;
   matrix_column< sparse_matrix_t > mc( *m_smatrix, col );
   matrix_column< sparse_matrix_t >::iterator ind;
   matrix_column< sparse_matrix_t >::iterator end = mc.end();
   for( ind = mc.begin() ; ind != end ; ++ind )
   {
      sum += *ind;
      count += 1;
   }
   return count;
}

int SparseMatrix::sumDiffColumns( size_t i, size_t j, double& sumdiff )
{
   unsigned int count = 0;
   sumdiff = 0;
   matrix_column< sparse_matrix_t > mcdiff( *m_smatrix, i );
   matrix_column< sparse_matrix_t > mcj( *m_smatrix, j );
   mcdiff -= mcj;
   matrix_column< sparse_matrix_t >::iterator ind;
   matrix_column< sparse_matrix_t >::iterator end = mcdiff.end();
   for( ind = mcdiff.begin() ; ind != end ; ++ind )
   {
      sumdiff += *ind;
      count += 1;
   }
   return count;
}

void SparseMatrix::print()
{
   cout << "matrix: " << *m_smatrix << endl;
}

