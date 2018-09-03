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
   DataFrame* m_pTestData;
   AlgUserAvg* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
};

PyTypeObject* UserAvgGetType();
PyObject* UserAvgTrain( PyUserAvg* self, PyObject* args, PyObject* kwdict );

#endif // __PY_USER_AVG_H__

