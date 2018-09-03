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
   DataFrame* m_pTestData;
   AlgFunkSvd* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
};

PyTypeObject* FunkSvdGetType();
PyObject* FunkSvdTrain( PyFunkSvd* self, PyObject* args, PyObject* kwdict );

#endif // __PY_FUNK_SVD_H__

