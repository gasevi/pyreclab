#include "PySlopeOne.h"
#include "PyCommon.h"
#include "SigHandler.h"
#include "DataWriter.h"
#include "MAP.h"
#include "NDCG.h"

#include <iostream>
#include <string>

using namespace std;

static
PyMethodDef SlopeOne_methods[] =
{
   { "train",     (PyCFunction)SlopeOneTrain,           METH_VARARGS|METH_KEYWORDS, "train model" },
   { "test",      (PyCFunction)PyTest<PySlopeOne>,      METH_VARARGS|METH_KEYWORDS, "test prediction model" },
   { "testrec",   (PyCFunction)PyTestrec<PySlopeOne>,   METH_VARARGS|METH_KEYWORDS, "test recommendation model" },
   { "predict",   (PyCFunction)PyPredict<PySlopeOne>,   METH_VARARGS,               "predict user's rating for an item" },
   { "recommend", (PyCFunction)PyRecommend<PySlopeOne>, METH_VARARGS|METH_KEYWORDS, "recommend ranked items to a user" },
   { "MAP",       (PyCFunction)PynDCG<PySlopeOne>,      METH_VARARGS|METH_KEYWORDS, "calculate Mean Average Precision for a user" },
   { "nDCG",      (PyCFunction)PyMAP<PySlopeOne>,       METH_VARARGS|METH_KEYWORDS, "calculate Normalized Discounted Cumulative Gain for a user" },
   { NULL, NULL, 0, NULL }
};

static PyTypeObject SlopeOneType =
{
#if PY_MAJOR_VERSION >= 3
   PyVarObject_HEAD_INIT(NULL, 0)
#else
   PyObject_HEAD_INIT( NULL )
   0,                                        // ob_size
#endif
   "libpyreclab.SlopeOne",                   // tp_name
   sizeof(PySlopeOne),                       // tp_basicsize
   0,                                        // tp_itemsize
   (destructor)PyDealloc<PySlopeOne>,        // tp_dealloc
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
   "SlopeOne objects",                       // tp_doc
   0,                                        // tp_traverse
   0,                                        // tp_clear
   0,                                        // tp_richcompare
   0,                                        // tp_weaklistoffset
   0,                                        // tp_iter
   0,                                        // tp_iternext
   SlopeOne_methods,                         // tp_methods
   0,                                        // tp_members
   0,                                        // tp_getset
   0,                                        // tp_base
   0,                                        // tp_dict
   0,                                        // tp_descr_get
   0,                                        // tp_descr_set
   0,                                        // tp_dictoffset
   0,                                        // tp_init
   0,                                        // tp_alloc
   PyNew<PySlopeOne, AlgSlopeOne>,           // tp_new
};


PyTypeObject* SlopeOneGetType()
{
   return &SlopeOneType;
}

PyObject* SlopeOneTrain( PySlopeOne* self, PyObject* args, PyObject* kwdict )
{
   SigHandler sigHandler( SIGINT );
   int cause = 0;
   cause = dynamic_cast<AlgSlopeOne*>( self->m_recAlgorithm )->train( sigHandler );

   if( AlgSlopeOne::STOPPED == cause )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_KeyboardInterrupt, "SIGINT received" );
      PyGILState_Release( gstate );
      return NULL;
   }

   Py_INCREF( Py_None );
   return Py_None;
}


