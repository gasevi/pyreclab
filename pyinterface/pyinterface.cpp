#include "DataWriter.h"
#include "PyMostPopular.h"
#include "PyUserAvg.h"
#include "PyItemAvg.h"
#include "PyUserKnn.h"
#include "PyItemKnn.h"
#include "PySlopeOne.h"
#include "PyFunkSvd.h"
#include "PyIFAls.h"

#include <iostream>
#include <signal.h>
#include <string>
#include <sstream>

using namespace std;

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
   if( PyType_Ready( MostPopular_getTypeObject() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( ItemAvg_getTypeObject() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( UserAvg_getTypeObject() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( UserKnn_getTypeObject() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( ItemKnn_getTypeObject() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( SlopeOne_getTypeObject() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( FunkSvd_getTypeObject() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( IFAls_getTypeObject() ) < 0 )
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

   Py_INCREF( MostPopular_getTypeObject() );
   PyModule_AddObject( module, "MostPopular", (PyObject*)MostPopular_getTypeObject() );
   Py_INCREF( UserAvg_getTypeObject() );
   PyModule_AddObject( module, "UserAvg", (PyObject*)UserAvg_getTypeObject() );
   Py_INCREF( ItemAvg_getTypeObject() );
   PyModule_AddObject( module, "ItemAvg", (PyObject*)ItemAvg_getTypeObject() );
   Py_INCREF( UserKnn_getTypeObject() );
   PyModule_AddObject( module, "UserKnn", (PyObject*)UserKnn_getTypeObject() );
   Py_INCREF( ItemKnn_getTypeObject() );
   PyModule_AddObject( module, "ItemKnn", (PyObject*)ItemKnn_getTypeObject() );
   Py_INCREF( SlopeOne_getTypeObject() );
   PyModule_AddObject( module, "SlopeOne", (PyObject*)SlopeOne_getTypeObject() );
   Py_INCREF( FunkSvd_getTypeObject() );
   PyModule_AddObject( module, "SVD", (PyObject*)FunkSvd_getTypeObject() );
   Py_INCREF( IFAls_getTypeObject() );
   PyModule_AddObject( module, "IFAls", (PyObject*)IFAls_getTypeObject() );

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}


