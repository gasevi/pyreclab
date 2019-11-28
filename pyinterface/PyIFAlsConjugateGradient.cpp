#include "PyIFAlsConjugateGradient.h"
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
PyMethodDef IFAlsConjugateGradient_methods[] =
{
   { "train",     (PyCFunction)IFAlsConjugateGradientTrain,           METH_VARARGS|METH_KEYWORDS, "train model" },
   { "reset",     (PyCFunction)PyReset<PyIFAlsConjugateGradient>,     METH_NOARGS,                "reset model parameters" },
   { "testrec",   (PyCFunction)PyTestrec<PyIFAlsConjugateGradient>,   METH_VARARGS|METH_KEYWORDS, "test recommendation model" },
   { "recommend", (PyCFunction)PyRecommend<PyIFAlsConjugateGradient>, METH_VARARGS|METH_KEYWORDS, "recommend ranked items to a user" },
   { "MAP",       (PyCFunction)PynDCG<PyIFAlsConjugateGradient>,      METH_VARARGS|METH_KEYWORDS, "calculate Mean Average Precision for a user" },
   { "nDCG",      (PyCFunction)PyMAP<PyIFAlsConjugateGradient>,       METH_VARARGS|METH_KEYWORDS, "calculate Normalized Discounted Cumulative Gain for a user" },
   { NULL, NULL, 0, NULL }
};

static PyTypeObject IFAlsConjugateGradientType =
{
#if PY_MAJOR_VERSION >= 3
   PyVarObject_HEAD_INIT(NULL, 0)
#else
   PyObject_HEAD_INIT( NULL )
   0,                                                                    // ob_size
#endif
   "libpyreclab.IFAlsConjugateGradient",                                 // tp_name
   sizeof(PyIFAlsConjugateGradient),                                     // tp_basicsize
   0,                                                                    // tp_itemsize
   (destructor)PyDealloc<PyIFAlsConjugateGradient>,                      // tp_dealloc
   0,                                                                    // tp_print
   0,                                                                    // tp_getattr
   0,                                                                    // tp_setattr
   0,                                                                    // tp_compare
   0,                                                                    // tp_repr
   0,                                                                    // tp_as_number
   0,                                                                    // tp_as_sequence
   0,                                                                    // tp_as_mapping
   0,                                                                    // tp_hash
   0,                                                                    // tp_call
   0,                                                                    // tp_str
   0,                                                                    // tp_getattro
   0,                                                                    // tp_setattro
   0,                                                                    // tp_as_buffer
   Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,                             // tp_flags
   "IFAlsConjugateGradient objects",                                     // tp_doc
   0,                                                                    // tp_traverse
   0,                                                                    // tp_clear
   0,                                                                    // tp_richcompare
   0,                                                                    // tp_weaklistoffset
   0,                                                                    // tp_iter
   0,                                                                    // tp_iternext
   IFAlsConjugateGradient_methods,                                       // tp_methods
   0,                                                                    // tp_members
   0,                                                                    // tp_getset
   0,                                                                    // tp_base
   0,                                                                    // tp_dict
   0,                                                                    // tp_descr_get
   0,                                                                    // tp_descr_set
   0,                                                                    // tp_dictoffset
   0,                                                                    // tp_init
   0,                                                                    // tp_alloc
   PyNewIFWFactors<PyIFAlsConjugateGradient, AlgIFAlsConjugateGradient>, // tp_new
};


PyTypeObject* IFAlsConjugateGradientGetType()
{
   return &IFAlsConjugateGradientType;
}

PyObject* IFAlsConjugateGradientTrain( PyIFAlsConjugateGradient* self, PyObject* args, PyObject* kwdict )
{
   int factors = -60223;
   size_t alsNumIter = 5;
   float lambda = 10;
   size_t cgNumIter = 2;

   static char* kwlist[] = { const_cast<char*>( "factors" ),
                             const_cast<char*>( "alsNumIter" ),
                             const_cast<char*>( "lambd" ),
                             const_cast<char*>( "cgNumIter" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "|iifi", kwlist, &factors, &alsNumIter, &lambda, &cgNumIter ) )
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
         cause = dynamic_cast<AlgIFAlsConjugateGradient*>( self->m_recAlgorithm )->train( alsNumIter, lambda, cgNumIter, sigHandler );
      }
      else
      {
         cout << "Warning: Train signature used is deprecated. From now on, 'factors' parameter should be provided in model's constructor. See documentation for more information." << endl;
         cause = dynamic_cast<AlgIFAlsConjugateGradient*>( self->m_recAlgorithm )->train( factors, alsNumIter, lambda, cgNumIter, sigHandler );
      }
   }
   catch( exception& e )
   {
      eMsg = e.what();
   }

   if( AlgIFAlsConjugateGradient::STOPPED == cause )
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


