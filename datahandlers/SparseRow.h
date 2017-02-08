#ifndef __SPARSE_ROW_H__
#define __SPARSE_ROW_H__

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>


class SparseMatrix;

class SparseRow
{
public:

   typedef boost::numeric::ublas::mapped_matrix<double> sparse_matrix_t;

   typedef boost::numeric::ublas::matrix_row< sparse_matrix_t > matrix_row_t;

   typedef boost::numeric::ublas::matrix_row< sparse_matrix_t >::iterator iterator;

   SparseRow();

   SparseRow( const SparseRow& srow );

   SparseRow( sparse_matrix_t& smatrix, size_t row );

   ~SparseRow();

   void set( size_t i, double value );

   double get( size_t i );

   size_t size();

   double mean();

   iterator begin();

   iterator end();

   void print();

private:

   matrix_row_t* m_prowmat;

};

#endif // __SPARSE_ROW_H__

