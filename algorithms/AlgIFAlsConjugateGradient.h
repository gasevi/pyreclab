#ifndef __ALG_IF_ALS_CONJUGATE_GRADIENT_H__
#define __ALG_IF_ALS_CONJUGATE_GRADIENT_H__

#include "DataReader.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>

#include <string>
#include <stdexcept>


class AlgIFAlsConjugateGradient
{
public:

   enum ETrainingEndCauses
   {
      FINISHED,
      STOPPED
   };

   AlgIFAlsConjugateGradient( DataReader& dreader,
                              int userpos = 0,
                              int itempos = 1,
                              int obspos = 2 );

   ~AlgIFAlsConjugateGradient();

   int train()
   throw( std::runtime_error& );

   int train( size_t factors, size_t maxiter, float lambda, size_t cgNumIter );

   double predict( size_t userrow, size_t itemcol );

   bool recommend( const std::string& userId,
                   size_t n,
                   std::vector<std::string>& ranking,
                   bool includeRated = false )
   throw( std::invalid_argument& );
 
   void stop()
   {
      m_running = false;
   }

private:

   void conjugateGradient( boost::numeric::ublas::matrix<double>& Y,
                           boost::numeric::ublas::matrix<double>& X,
                           boost::numeric::ublas::diagonal_matrix<double>** Cu,
                           size_t numIter );

   void conjugateGradientSlow( boost::numeric::ublas::matrix<double>& Y,
                               boost::numeric::ublas::matrix<double>& X,
                               boost::numeric::ublas::diagonal_matrix<double>** Cu,
                               boost::numeric::ublas::mapped_vector<double>** Pu,
                               boost::numeric::ublas::identity_matrix<double>& Iu,
                               size_t numIter );

   void reset( size_t factors, size_t maxiter, float lambda, size_t cgNumIter );

   bool m_running;

   size_t m_nfactors;

   size_t m_alsNumIter;

   double m_lambda;

   float m_alpha;

   size_t m_cgNumIter;

   std::map<std::string, size_t> m_fUserMapper;

   std::map<size_t, std::string> m_bUserMapper;

   std::map<std::string, size_t> m_fItemMapper;

   std::map<size_t, std::string> m_bItemMapper;

   boost::numeric::ublas::mapped_vector<double>** m_pVecPu;

   boost::numeric::ublas::mapped_vector<double>** m_pVecPi;

   boost::numeric::ublas::diagonal_matrix<double>** m_pMatCu;

   boost::numeric::ublas::diagonal_matrix<double>** m_pMatCi;

   boost::numeric::ublas::matrix<double> m_Xu;

   boost::numeric::ublas::matrix<double> m_Yi;

};

#endif // __ALG_IF_ALS_CONJUGATE_GRADIENT_H__

