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


