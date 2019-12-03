#ifndef __ALG_IF_ALS_H__
#define __ALG_IF_ALS_H__

#include "DataReader.h"
#include "FlowControl.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>

#include <string>
#include <stdexcept>


class AlgIFAls
{
public:

   enum ETrainingEndCauses
   {
      FINISHED,
      STOPPED
   };

   AlgIFAls( DataReader& dreader,
             int userpos = 0,
             int itempos = 1,
             int obspos = 2 );

   AlgIFAls( size_t factors,
             DataReader& dreader,
             int userpos = 0,
             int itempos = 1,
             int obspos = 2 );

   ~AlgIFAls();

   int train( size_t factors, size_t alsNumIter, float lambda, FlowControl& fcontrol, bool progress );

   int train( size_t alsNumIter, float lambda, FlowControl& fcontrol, bool progress );

   void reset();

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

   int train( FlowControl& fcontrol, bool progress )
   throw( std::runtime_error& );

   void reset( size_t factors, size_t maxiter, float lambda );

   bool invert( const boost::numeric::ublas::matrix<double>& input,
                boost::numeric::ublas::matrix<double>& inverse );

   bool m_running;

   size_t m_nfactors;

   size_t m_alsNumIter;

   float m_alpha;

   double m_lambda;

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

#endif // __ALG_IF_ALS_H__

