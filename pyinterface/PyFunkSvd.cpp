#include "PyFunkSvd.h"
#include "PyCommon.h"
#include "SigHandler.h"
#include "DataWriter.h"
#include "MAP.h"
#include "NDCG.h"

#include <iostream>
#include <string>

using namespace std;

static
PyMethodDef FunkSvd_methods[] =
{
   { "train",     (PyCFunction)FunkSvdTrain,           METH_VARARGS|METH_KEYWORDS, "train model" },
   { "reset",     (PyCFunction)PyReset<PyFunkSvd>,     METH_NOARGS,                "reset model parameters" },
   { "loss",      (PyCFunction)PyLoss<PyFunkSvd>,      METH_NOARGS,                "returns current loss value" },
   { "test",      (PyCFunction)PyTest<PyFunkSvd>,      METH_VARARGS|METH_KEYWORDS, "test prediction model" },
   { "testrec",   (PyCFunction)PyTestrec<PyFunkSvd>,   METH_VARARGS|METH_KEYWORDS, "test recommendation model" },
   { "predict",   (PyCFunction)PyPredict<PyFunkSvd>,   METH_VARARGS,               "predict user's rating for an item" },
   { "recommend", (PyCFunction)PyRecommend<PyFunkSvd>, METH_VARARGS|METH_KEYWORDS, "recommend ranked items to a user" },
   { "precision", (PyCFunction)PyPrecision<PyFunkSvd>, METH_VARARGS|METH_KEYWORDS, "calculate Precision for a user" },
   { "recall",    (PyCFunction)PyRecall<PyFunkSvd>,    METH_VARARGS|METH_KEYWORDS, "calculate Recall for a user" },
   { "AUC",       (PyCFunction)PyAUC<PyFunkSvd>,       METH_VARARGS|METH_KEYWORDS, "calculate Area Under the ROC Curve for a user" },
   { "MAP",       (PyCFunction)PynDCG<PyFunkSvd>,      METH_VARARGS|METH_KEYWORDS, "calculate Mean Average Precision for a user" },
   { "nDCG",      (PyCFunction)PyMAP<PyFunkSvd>,       METH_VARARGS|METH_KEYWORDS, "calculate Normalized Discounted Cumulative Gain for a user" },
   { NULL, NULL, 0, NULL }
};

static PyTypeObject FunkSvdType =
{
#if PY_MAJOR_VERSION >= 3
   PyVarObject_HEAD_INIT(NULL, 0)
#else
   PyObject_HEAD_INIT( NULL )
   0,                                        // ob_size
#endif
   "libpyreclab.FunkSvd",                    // tp_name
   sizeof(PyFunkSvd),                        // tp_basicsize
   0,                                        // tp_itemsize
   (destructor)PyDealloc<PyFunkSvd>,         // tp_dealloc
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
   "FunkSvd objects",                        // tp_doc
   0,                                        // tp_traverse
   0,                                        // tp_clear
   0,                                        // tp_richcompare
   0,                                        // tp_weaklistoffset
   0,                                        // tp_iter
   0,                                        // tp_iternext
   FunkSvd_methods,                          // tp_methods
   0,                                        // tp_members
   0,                                        // tp_getset
   0,                                        // tp_base
   0,                                        // tp_dict
   0,                                        // tp_descr_get
   0,                                        // tp_descr_set
   0,                                        // tp_dictoffset
   0,                                        // tp_init
   0,                                        // tp_alloc
   FunkSvdNew,                               // tp_new
};


PyTypeObject* FunkSvdGetType()
{
   return &FunkSvdType;
}

PyObject* FunkSvdNew( PyTypeObject* type, PyObject* args, PyObject* kwdict )
{
   int factors = -60223;
   const char* dsfilename = NULL;
   char dlmchar = ',';
   int header = 0;
   int usercol = 0;
   int itemcol = 1;
   int ratingcol = 2;

   static char* kwlist[] = { const_cast<char*>( "dataset" ),
                             const_cast<char*>( "dlmchar" ),
                             const_cast<char*>( "header" ),
                             const_cast<char*>( "usercol" ),
                             const_cast<char*>( "itemcol" ),
                             const_cast<char*>( "ratingcol" ),
                             const_cast<char*>( "factors" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "s|ciiiii", kwlist, &dsfilename,
                                     &dlmchar, &header, &usercol, &itemcol, &ratingcol, &factors ) )
   {
      return NULL;
   }

   if( NULL == dsfilename )
   {
      return NULL;
   }

   PyFunkSvd* self = reinterpret_cast<PyFunkSvd*>( type->tp_alloc( type, 0 ) );
   if( self != NULL )
   {
      self->m_trainingReader = new DataReader( dsfilename, dlmchar, header );
      if( NULL == self->m_trainingReader )
      {
         Py_DECREF( self );
         return NULL;
      }

      if( factors < 0 )
      {
         cout << "Warning: Constructor signature used is deprecated. From now on, it should include 'factors' parameter. See documentation for more information." << endl;
         self->m_recAlgorithm = new AlgFunkSvd( *self->m_trainingReader, usercol, itemcol, ratingcol );
      }
      else
      {
         self->m_recAlgorithm = new AlgFunkSvd( factors, *self->m_trainingReader, usercol, itemcol, ratingcol );
      }
      if( NULL == self->m_recAlgorithm )
      {
         Py_DECREF( self );
         return NULL;
      }
   }

   return reinterpret_cast<PyObject*>( self );
}

PyObject* FunkSvdTrain( PyFunkSvd* self, PyObject* args, PyObject* kwdict )
{
   int factors = -60223;
   size_t maxiter = 100;
   float lr = 0.01;
   float lambda = 0.1;
   int progress = 0;

   static char* kwlist[] = { const_cast<char*>( "factors" ),
                             const_cast<char*>( "maxiter" ),
                             const_cast<char*>( "lr" ),
                             const_cast<char*>( "lamb" ),
                             const_cast<char*>( "progress" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "|iiffi", kwlist, &factors, &maxiter, &lr, &lambda, &progress ) )
   {
      return NULL;
   }

   SigHandler sigHandler( SIGINT );
   int cause = 0;
   if( factors < 0 )
   {
      cause = dynamic_cast<AlgFunkSvd*>( self->m_recAlgorithm )->train( maxiter, lr, lambda, sigHandler, progress );
   }
   else
   {
      cout << "Warning: Train signature used is deprecated. From now on, 'factors' parameter should be provided in model's constructor. See documentation for more information." << endl;
      cause = dynamic_cast<AlgFunkSvd*>( self->m_recAlgorithm )->train( factors, maxiter, lr, lambda, sigHandler, progress );
   }

   if( AlgFunkSvd::STOPPED == cause )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_KeyboardInterrupt, "SIGINT received" );
      PyGILState_Release( gstate );
      return NULL;
   }

   Py_INCREF( Py_None );
   return Py_None;
}


