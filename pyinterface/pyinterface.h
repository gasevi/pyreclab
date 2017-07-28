#ifndef __PY_INTERFACE_H__
#define __PY_INTERFACE_H__

#include <Python.h> // This header was set at the first place to avoid warnings
#include <signal.h>

#include "DataReader.h"
#include "RatingMatrix.h"
#include "RecSysAlgorithm.h"
#include "MAE.h"
#include "RMSE.h"


class PrlSigHandler
{
public:

   enum EAlgType
   {
      UNKNOWN,
      FUNK_SVD,
      ITEM_AVG,
      ITEM_KNN,
      MOST_POPULAR,
      SLOPE_ONE,
      USER_AVG,
      USER_KNN
   };

   static
   void handler( int signum );

   static
   void registerObj( PyObject* obj, EAlgType type );

   static
   void unregisterObj();

   static
   struct sigaction* handlesignal( int signum );

   static
   void restoresignal( int signum, struct sigaction* pAction );

private:

   static
   EAlgType m_algType;

   static
   PyObject* m_activeObj;

};

/*
typedef struct RecSysStruct
{
   PyObject_HEAD
   DataReader* m_trainingReader;
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
*/

#endif // __PY_INTERFACE_H__

