#include "PyIFAls.h"
#include "PyCommon.h"
#include "SigHandler.h"
#include "DataWriter.h"
#include "DataFrame.h"
#include "MAP.h"
#include "NDCG.h"

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

static
PyMethodDef IFAls_methods[] =
{
   { "train",     (PyCFunction)IFAlsTrain,           METH_VARARGS|METH_KEYWORDS, "train model" },
   { "reset",     (PyCFunction)PyReset<PyIFAls>,     METH_NOARGS,                "reset model parameters" },
   { "testrec",   (PyCFunction)PyTestrec<PyIFAls>,   METH_VARARGS|METH_KEYWORDS, "test recommendation model" },
   { "recommend", (PyCFunction)PyRecommend<PyIFAls>, METH_VARARGS|METH_KEYWORDS, "recommend ranked items to a user" },
   { "precision", (PyCFunction)PyPrecision<PyIFAls>, METH_VARARGS|METH_KEYWORDS, "calculate Precision for a user" },
   { "recall",    (PyCFunction)PyRecall<PyIFAls>,    METH_VARARGS|METH_KEYWORDS, "calculate Recall for a user" },
   { "AUC",       (PyCFunction)PyAUC<PyIFAls>,       METH_VARARGS|METH_KEYWORDS, "calculate Area Under the ROC Curve for a user" },
   { "MAP",       (PyCFunction)PynDCG<PyIFAls>,      METH_VARARGS|METH_KEYWORDS, "calculate Mean Average Precision for a user" },
   { "nDCG",      (PyCFunction)PyMAP<PyIFAls>,       METH_VARARGS|METH_KEYWORDS, "calculate Normalized Discounted Cumulative Gain for a user" },
   { NULL, NULL, 0, NULL }
};

static PyTypeObject IFAlsType =
{
#if PY_MAJOR_VERSION >= 3
   PyVarObject_HEAD_INIT(NULL, 0)
#else
   PyObject_HEAD_INIT( NULL )
   0,                                        // ob_size
#endif
   "libpyreclab.IFAls",                      // tp_name
   sizeof(PyIFAls),                          // tp_basicsize
   0,                                        // tp_itemsize
   (destructor)PyDealloc<PyIFAls>,           // tp_dealloc
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
   "IFAls objects",                          // tp_doc
   0,                                        // tp_traverse
   0,                                        // tp_clear
   0,                                        // tp_richcompare
   0,                                        // tp_weaklistoffset
   0,                                        // tp_iter
   0,                                        // tp_iternext
   IFAls_methods,                            // tp_methods
   0,                                        // tp_members
   0,                                        // tp_getset
   0,                                        // tp_base
   0,                                        // tp_dict
   0,                                        // tp_descr_get
   0,                                        // tp_descr_set
   0,                                        // tp_dictoffset
   0,                                        // tp_init
   0,                                        // tp_alloc
   PyNewIFWFactors<PyIFAls, AlgIFAls>,       // tp_new
};


PyTypeObject* IFAlsGetType()
{
   return &IFAlsType;
}

PyObject* IFAlsTrain( PyIFAls* self, PyObject* args, PyObject* kwdict )
{
   int factors = -60223;
   size_t alsNumIter = 5;
   float lambda = 10;
   int progress = 0;

   static char* kwlist[] = { const_cast<char*>( "factors" ),
                             const_cast<char*>( "alsNumIter" ),
                             const_cast<char*>( "lambd" ),
                             const_cast<char*>( "progress" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "|iifi", kwlist, &factors, &alsNumIter, &lambda, &progress ) )
   {
      return NULL;
   }

   SigHandler sigHandler( SIGINT );
   int cause = -1;
   string eMsg;
   try
   {
      if( factors < 0 )
      {
         cause = dynamic_cast<AlgIFAls*>( self->m_recAlgorithm )->train( alsNumIter, lambda, sigHandler, progress );
      }
      else
      {
         cout << "Warning: Train signature used is deprecated. From now on, 'factors' parameter should be provided in model's constructor. See documentation for more information." << endl;
         cause = dynamic_cast<AlgIFAls*>( self->m_recAlgorithm )->train( factors, alsNumIter, lambda, sigHandler, progress );
      }
   }
   catch( exception& e )
   {
      eMsg = e.what();
   }

   if( AlgIFAls::STOPPED == cause )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_KeyboardInterrupt, "SIGINT received" );
      PyGILState_Release( gstate );
      return NULL;
   }
   else if( 0 > cause )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_RuntimeError, eMsg.c_str() );
      PyGILState_Release( gstate );
      return NULL;
   }

   Py_INCREF( Py_None );
   return Py_None;
}


