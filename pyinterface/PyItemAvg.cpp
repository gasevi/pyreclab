#include "PyItemAvg.h"
#include "PyCommon.h"
#include "SigHandler.h"
#include "DataWriter.h"
#include "MAP.h"
#include "NDCG.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

static
PyMethodDef ItemAvg_methods[] =
{  
   { "train",     (PyCFunction)ItemAvg_train,          METH_VARARGS|METH_KEYWORDS, "train model" },
   { "test",      (PyCFunction)PyTest<PyItemAvg>,      METH_VARARGS|METH_KEYWORDS, "test prediction model" },
   { "testrec",   (PyCFunction)PyTestrec<PyItemAvg>,   METH_VARARGS|METH_KEYWORDS, "test recommendation model" },
   { "predict",   (PyCFunction)PyPredict<PyItemAvg>,   METH_VARARGS,               "predict user's rating for an item" },
   { "recommend", (PyCFunction)PyRecommend<PyItemAvg>, METH_VARARGS|METH_KEYWORDS, "recommend ranked items to a user" },
   { "precision", (PyCFunction)PyPrecision<PyItemAvg>, METH_VARARGS|METH_KEYWORDS, "calculate Precision for a user" },
   { "recall",    (PyCFunction)PyRecall<PyItemAvg>,    METH_VARARGS|METH_KEYWORDS, "calculate Recall for a user" },
   { "AUC",       (PyCFunction)PyAUC<PyItemAvg>,       METH_VARARGS|METH_KEYWORDS, "calculate Area Under the ROC Curve for a user" },
   { "MAP",       (PyCFunction)PynDCG<PyItemAvg>,      METH_VARARGS|METH_KEYWORDS, "calculate Mean Average Precision for a user" },
   { "nDCG",      (PyCFunction)PyMAP<PyItemAvg>,       METH_VARARGS|METH_KEYWORDS, "calculate Normalized Discounted Cumulative Gain for a user" },
   { NULL, NULL, 0, NULL }
};

static PyTypeObject ItemAvgType =
{
#if PY_MAJOR_VERSION >= 3
   PyVarObject_HEAD_INIT(NULL, 0)
#else
   PyObject_HEAD_INIT( NULL )
   0,                                        // ob_size
#endif
   "libpyreclab.ItemAvg",                    // tp_name
   sizeof(PyItemAvg),                        // tp_basicsize
   0,                                        // tp_itemsize
   (destructor)PyDealloc<PyItemAvg>,         // tp_dealloc
   0,                                        // tp_print
   0,                                        // tp_getattr
   0,                                        // tp_setattr
   0,                                        // tp_compare
   0,                                        // tp_repr
   0,                                        // tp_as_number
   0,                                        // tp_as_sequence
   0,                                        // tp_as_mapping
   0,                                        // tp_hash
   0,                                        // tp_call
   0,                                        // tp_str
   0,                                        // tp_getattro
   0,                                        // tp_setattro
   0,                                        // tp_as_buffer
   Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // tp_flags
   "ItemAvg objects",                        // tp_doc
   0,                                        // tp_traverse
   0,                                        // tp_clear
   0,                                        // tp_richcompare
   0,                                        // tp_weaklistoffset
   0,                                        // tp_iter
   0,                                        // tp_iternext
   ItemAvg_methods,                          // tp_methods
   0,                                        // tp_members
   0,                                        // tp_getset
   0,                                        // tp_base
   0,                                        // tp_dict
   0,                                        // tp_descr_get
   0,                                        // tp_descr_set
   0,                                        // tp_dictoffset
   0,                                        // tp_init
   0,                                        // tp_alloc
   PyNew<PyItemAvg, AlgItemAvg>,             // tp_new
};


PyTypeObject* ItemAvgGetType()
{
   return &ItemAvgType;
}

PyObject* ItemAvg_train( PyItemAvg* self, PyObject* args, PyObject* kwdict )
{
   int progress = 0;

   static char* kwlist[] = { const_cast<char*>( "progress" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "|i", kwlist, &progress ) )
   {
      return NULL;
   }

   SigHandler sigHandler( SIGINT );
   int cause = 0;
   cause = dynamic_cast<AlgItemAvg*>( self->m_recAlgorithm )->train( sigHandler, progress );

   if( AlgItemAvg::STOPPED == cause )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_KeyboardInterrupt, "SIGINT received" );
      PyGILState_Release( gstate );
      return NULL;
   }

   Py_INCREF( Py_None );
   return Py_None;
}


