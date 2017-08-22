#include "pyinterface.h"
#include "DataWriter.h"
#include "PyMostPopular.h"
#include "PyUserAvg.h"
#include "PyItemAvg.h"
#include "PyUserKnn.h"
#include "PyItemKnn.h"
#include "PySlopeOne.h"
#include "PyFunkSvd.h"

#include <iostream>
#include <signal.h>
#include <string>
#include <sstream>

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

#if PY_MAJOR_VERSION >= 3
struct module_state
{
   PyObject* error;
};
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#endif

static PyMethodDef libpyreclabMethods[] =
{
   { NULL, NULL }
};

#if PY_MAJOR_VERSION >= 3
static int libpyreclab_traverse(PyObject *m, visitproc visit, void *arg)
{
   Py_VISIT( GETSTATE( m )->error );
   return 0;
}

static int libpyreclab_clear( PyObject *m )
{
   Py_CLEAR( GETSTATE( m )->error );
   return 0;
}

static struct PyModuleDef moduledef =
{
   PyModuleDef_HEAD_INIT,
   "libpyreclab",
   NULL,
   sizeof( struct module_state ),
   libpyreclabMethods,
   NULL,
   libpyreclab_traverse,
   libpyreclab_clear,
   NULL
};

PyMODINIT_FUNC PyInit_libpyreclab()
#else
extern "C" void initlibpyreclab()
#endif
{
   if( PyType_Ready( &MostPopularType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( &ItemAvgType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( &UserAvgType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( &UserKnnType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( &ItemKnnType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( &SlopeOneType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( &FunkSvdType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

#if PY_MAJOR_VERSION >= 3
   PyObject* module = PyModule_Create( &moduledef );
#else
   PyObject* module = Py_InitModule( "libpyreclab", libpyreclabMethods );
#endif
   if( NULL == module )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   Py_INCREF( &MostPopularType );
   PyModule_AddObject( module, "MostPopular", (PyObject*)&MostPopularType );
   Py_INCREF( &UserAvgType );
   PyModule_AddObject( module, "UserAvg", (PyObject*)&UserAvgType );
   Py_INCREF( &ItemAvgType );
   PyModule_AddObject( module, "ItemAvg", (PyObject*)&ItemAvgType );
   Py_INCREF( &UserKnnType );
   PyModule_AddObject( module, "UserKnn", (PyObject*)&UserKnnType );
   Py_INCREF( &ItemKnnType );
   PyModule_AddObject( module, "ItemKnn", (PyObject*)&ItemKnnType );
   Py_INCREF( &SlopeOneType );
   PyModule_AddObject( module, "SlopeOne", (PyObject*)&SlopeOneType );
   Py_INCREF( &FunkSvdType );
   PyModule_AddObject( module, "SVD", (PyObject*)&FunkSvdType );

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}


