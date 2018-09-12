#ifndef __PY_ITEM_AVG_H__
#define __PY_ITEM_AVG_H__

#include <Python.h> // This header was set at the first place to avoid warnings

#include "AlgItemAvg.h"
#include "DataReader.h"
#include "MAE.h"
#include "RMSE.h"

class PyItemAvg
{
public:

   PyObject_HEAD
   DataReader* m_trainingReader;
   DataFrame* m_pTestData;
   AlgItemAvg* m_recAlgorithm;
};

PyTypeObject* ItemAvgGetType();
PyObject* ItemAvg_train( PyItemAvg* self, PyObject* args, PyObject* kwdict );

#endif // __PY_ITEM_AVG_H__

