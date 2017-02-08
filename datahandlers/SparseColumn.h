#ifndef __SPARSE_COLUMN_H__
#define __SPARSE_COLUMN_H__

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

class SparseMatrix;

class SparseColumn
{
public:

   typedef boost::numeric::ublas::mapped_matrix<double> sparse_matrix_t;

   typedef boost::numeric::ublas::matrix_column< sparse_matrix_t > matrix_column_t;

   typedef boost::numeric::ublas::matrix_column< sparse_matrix_t >::iterator iterator;

   SparseColumn();

   SparseColumn( const SparseColumn& scol );

   SparseColumn( sparse_matrix_t& smatrix, size_t column );

   ~SparseColumn();

   double get( size_t i );

   size_t size();

   double mean();

   iterator begin();

   iterator end();

   void print();

private:

   matrix_column_t* m_pcolmat;

};

#endif // __SPARSE_COLUMN_H__
