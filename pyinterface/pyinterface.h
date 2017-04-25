#ifndef __PY_INTERFACE_H__
#define __PY_INTERFACE_H__

#include "DataReader.h"
#include "RatingMatrix.h"
#include "RecSysAlgorithm.h"
#include "MAE.h"
#include "RMSE.h"

#include <Python.h>
#include <signal.h>

typedef struct RecSysStruct
{
   PyObject_HEAD
   DataReader* m_trainingReader;
   RatingMatrix* m_ratingMatrix;
   RecSysAlgorithm* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
   static
   RecSysAlgorithm* m_currentRecSys;

   static
   void sighandler( int signum )
   {
      if( NULL != RecSysStruct::m_currentRecSys )
      {
         RecSysStruct::m_currentRecSys->stop();
      }
   }

   static
   struct sigaction* handlesignal( int signum )
   {
      struct sigaction newAction;
      struct sigaction* pOldAction;
      newAction.sa_handler = sighandler;
      sigemptyset( &newAction.sa_mask );
      newAction.sa_flags = 0;
      sigaction( signum, &newAction, pOldAction );
      return pOldAction;
   }

   static
   void restoresignal( int signum, struct sigaction* pAction )
   {
      sigaction( SIGINT, NULL, pAction );
   }

} Recommender;

void Recommender_dealloc( Recommender* self );
PyObject* Recommender_train( Recommender* self, PyObject* args );
PyObject* Recommender_predict( Recommender* self, PyObject* args, PyObject* kwdict );
PyObject* Recommender_recommend( Recommender* self, PyObject* args, PyObject* kwds );
PyObject* Recommender_test( Recommender* self, PyObject* args, PyObject* kwdict );
PyObject* Recommender_testrec( Recommender* self, PyObject* args, PyObject* kwdict );

#endif // __PY_INTERFACE_H__

