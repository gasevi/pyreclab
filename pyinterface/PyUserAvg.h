#ifndef __PY_USER_AVG_H__
#define __PY_USER_AVG_H__

#include <Python.h> // This header was set at the first place to avoid warnings

#include "AlgUserAvg.h"
#include "DataReader.h"
#include "MAE.h"
#include "RMSE.h"

class PyUserAvg
{
public:

   PyObject_HEAD
   DataReader* m_trainingReader;
   AlgUserAvg* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
};

PyObject* UserAvg_new( PyTypeObject* type, PyObject* args, PyObject* kwdict );
void UserAvg_dealloc( PyUserAvg* self );
PyObject* UserAvg_train( PyUserAvg* self, PyObject* args );
PyObject* UserAvg_predict( PyUserAvg* self, PyObject* args );
PyObject* UserAvg_recommend( PyUserAvg* self, PyObject* args, PyObject* kwdict );
PyObject* UserAvg_test( PyUserAvg* self, PyObject* args, PyObject* kwdict );
PyObject* UserAvg_testrec( PyUserAvg* self, PyObject* args, PyObject* kwdict );

static
PyMethodDef UserAvg_methods[] =
{
   { "train",     (PyCFunction)UserAvg_train,     METH_NOARGS,                "train model" },
   { "test",      (PyCFunction)UserAvg_test,      METH_VARARGS|METH_KEYWORDS, "test model" },
   { "testrec",   (PyCFunction)UserAvg_testrec,   METH_VARARGS|METH_KEYWORDS, "test recommendation model" },
   { "predict",   (PyCFunction)UserAvg_predict,   METH_VARARGS,               "predict user's rating for an item" },
   { "recommend", (PyCFunction)UserAvg_recommend, METH_VARARGS|METH_KEYWORDS, "recommend ranked items to a user" },
   { NULL }
};

static PyTypeObject UserAvgType =
{
#if PY_MAJOR_VERSION >= 3
   PyVarObject_HEAD_INIT(NULL, 0)
#else
   PyObject_HEAD_INIT( NULL )
   0,                                        /* ob_size */
#endif
   "libpyreclab.UserAvg",                    /* tp_name */
   sizeof(PyUserAvg),                          /* tp_basicsize */
   0,                                        /* tp_itemsize */
   (destructor)UserAvg_dealloc,              /* tp_dealloc */
   0,                                        /* tp_print */
   0,                                        /* tp_getattr */
   0,                                        /* tp_setattr */
   0,                                        /* tp_compare */
   0,                                        /* tp_repr */
   0,                                        /* tp_as_number */
   0,                                        /* tp_as_sequence */
   0,                                        /* tp_as_mapping */
   0,                                        /* tp_hash */
   0,                                        /* tp_call */
   0,                                        /* tp_str */
   0,                                        /* tp_getattro */
   0,                                        /* tp_setattro */
   0,                                        /* tp_as_buffer */
   Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
   "UserAvg objects",                        /* tp_doc */
   0,                                        /* tp_traverse */
   0,                                        /* tp_clear */
   0,                                        /* tp_richcompare */
   0,                                        /* tp_weaklistoffset */
   0,                                        /* tp_iter */
   0,                                        /* tp_iternext */
   UserAvg_methods,                          /* tp_methods */
   0,                                        /* tp_members */
   0,                                        /* tp_getset */
   0,                                        /* tp_base */
   0,                                        /* tp_dict */
   0,                                        /* tp_descr_get */
   0,                                        /* tp_descr_set */
   0,                                        /* tp_dictoffset */
   0,                                        /* tp_init */
   0,                                        /* tp_alloc */
   UserAvg_new,                              /* tp_new */
};


#endif // __PY_USER_AVG_H__

