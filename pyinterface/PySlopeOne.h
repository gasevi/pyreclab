#ifndef __PY_SLOPE_ONE_H__
#define __PY_SLOPE_ONE_H__

#include <Python.h> // This header was set at the first place to avoid warnings

#include "AlgSlopeOne.h"
#include "DataReader.h"
#include "MAE.h"
#include "RMSE.h"

class PySlopeOne
{
public:

   PyObject_HEAD
   DataReader* m_trainingReader;
   DataFrame* m_pTestData;
   AlgSlopeOne* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
};

PyTypeObject* SlopeOneGetType();
PyObject* SlopeOneTrain( PySlopeOne* self, PyObject* args, PyObject* kwdict );

#endif // __PY_SLOPE_ONE_H__

