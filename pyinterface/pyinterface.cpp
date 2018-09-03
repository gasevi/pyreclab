#include "DataWriter.h"
#include "PyMostPopular.h"
#include "PyUserAvg.h"
#include "PyItemAvg.h"
#include "PyUserKnn.h"
#include "PyItemKnn.h"
#include "PySlopeOne.h"
#include "PyFunkSvd.h"
#include "PyIFAls.h"
#include "PyIFAlsConjugateGradient.h"

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
   if( PyType_Ready( MostPopularGetType() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( ItemAvgGetType() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( UserAvgGetType() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( UserKnnGetType() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( ItemKnnGetType() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( SlopeOneGetType() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( FunkSvdGetType() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( IFAlsGetType() ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( IFAlsConjugateGradientGetType() ) < 0 )
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

   Py_INCREF( MostPopularGetType() );
   PyModule_AddObject( module, "MostPopular", (PyObject*)MostPopularGetType() );
   Py_INCREF( UserAvgGetType() );
   PyModule_AddObject( module, "UserAvg", (PyObject*)UserAvgGetType() );
   Py_INCREF( ItemAvgGetType() );
   PyModule_AddObject( module, "ItemAvg", (PyObject*)ItemAvgGetType() );
   Py_INCREF( UserKnnGetType() );
   PyModule_AddObject( module, "UserKnn", (PyObject*)UserKnnGetType() );
   Py_INCREF( ItemKnnGetType() );
   PyModule_AddObject( module, "ItemKnn", (PyObject*)ItemKnnGetType() );
   Py_INCREF( SlopeOneGetType() );
   PyModule_AddObject( module, "SlopeOne", (PyObject*)SlopeOneGetType() );
   Py_INCREF( FunkSvdGetType() );
   PyModule_AddObject( module, "SVD", (PyObject*)FunkSvdGetType() );
   Py_INCREF( IFAlsGetType() );
   PyModule_AddObject( module, "IFAls", (PyObject*)IFAlsGetType() );
   Py_INCREF( IFAlsConjugateGradientGetType() );
   PyModule_AddObject( module, "IFAlsConjugateGradient", (PyObject*)IFAlsConjugateGradientGetType() );

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}


