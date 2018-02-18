#include "PrlSigHandler.h"
#include "PyMostPopular.h"
#include "PyUserAvg.h"
#include "PyItemAvg.h"
#include "PyUserKnn.h"
#include "PyItemKnn.h"
#include "PySlopeOne.h"
#include "PyFunkSvd.h"
#include "PyIFAls.h"

using namespace std;

PyObject* PrlSigHandler::m_activeObj = NULL;
PrlSigHandler::EAlgType PrlSigHandler::m_algType = PrlSigHandler::UNKNOWN;

void PrlSigHandler::handler( int signum )
{
   if( NULL != m_activeObj )
   {
      switch( m_algType )
      {
      case FUNK_SVD:
         reinterpret_cast<PyFunkSvd*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      case ITEM_AVG:
         reinterpret_cast<PyItemAvg*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      case ITEM_KNN:
         reinterpret_cast<PyItemKnn*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      case MOST_POPULAR:
         reinterpret_cast<PyMostPopular*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      case SLOPE_ONE:
         reinterpret_cast<PySlopeOne*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      case USER_AVG:
         reinterpret_cast<PyUserAvg*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      case USER_KNN:
         reinterpret_cast<PyUserKnn*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      case IF_ALS:
         reinterpret_cast<PyIFAls*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      default:
         break;
      }
   }
}

void PrlSigHandler::registerObj( PyObject* obj, EAlgType type )
{
   m_algType = type;
   m_activeObj = obj;
}

void PrlSigHandler::unregisterObj()
{
   m_activeObj = NULL;
}

struct sigaction* PrlSigHandler::handlesignal( int signum )
{
   struct sigaction newAction;
   struct sigaction* pOldAction = NULL;
   newAction.sa_handler = PrlSigHandler::handler;
   sigemptyset( &newAction.sa_mask );
   newAction.sa_flags = 0;
   sigaction( signum, &newAction, pOldAction );
   return pOldAction;
}

void PrlSigHandler::restoresignal( int signum, struct sigaction* pAction )
{
   sigaction( SIGINT, NULL, pAction );
}


