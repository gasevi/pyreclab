#ifndef __PY_IF_ALS_H__
#define __PY_IF_ALS_H__

#include <Python.h> // This header was set at the first place to avoid warnings

#include "AlgIFAls.h"
#include "DataReader.h"
#include "DataFrame.h"
#include "MAE.h"
#include "RMSE.h"

class PyIFAls
{
public:

   PyObject_HEAD
   DataReader* m_trainingReader;
   DataFrame* m_pTestData;
   AlgIFAls* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
};

PyTypeObject* IFAlsGetType();
PyObject* IFAlsNew( PyTypeObject* type, PyObject* args, PyObject* kwdict );
PyObject* IFAlsTrain( PyIFAls* self, PyObject* args, PyObject* kwdict );

#endif // __PY_IF_ALS_H__

