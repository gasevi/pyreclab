#ifndef __PY_FUNK_SVD_H__
#define __PY_FUNK_SVD_H__

#include <Python.h> // This header was set at the first place to avoid warnings

#include "AlgFunkSvd.h"
#include "DataReader.h"
#include "MAE.h"
#include "RMSE.h"

class PyFunkSvd
{
public:

   PyObject_HEAD
   DataReader* m_trainingReader;
   int m_nusers;
   int m_nitems;
   DataFrame* m_pTestData;
   AlgFunkSvd* m_recAlgorithm;
};

PyTypeObject* FunkSvdGetType();
PyObject* FunkSvdNew( PyTypeObject* type, PyObject* args, PyObject* kwdict );
PyObject* FunkSvdTrain( PyFunkSvd* self, PyObject* args, PyObject* kwdict );

#endif // __PY_FUNK_SVD_H__

