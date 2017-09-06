#ifndef __PY_MOST_POPULAR_H__
#define __PY_MOST_POPULAR_H__

#include <Python.h> // This header was set at the first place to avoid warnings

#include "AlgMostPopular.h"
#include "DataReader.h"
#include "MAE.h"
#include "RMSE.h"

class PyMostPopular
{
public:

   PyObject_HEAD
   DataReader* m_trainingReader;
   AlgMostPopular* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
};

PyTypeObject* MostPopular_getTypeObject();
PyObject* MostPopular_new( PyTypeObject* type, PyObject* args, PyObject* kwdict );
void MostPopular_dealloc( PyMostPopular* self );
PyObject* MostPopular_train( PyMostPopular* self, PyObject* args, PyObject* kwdict );
PyObject* MostPopular_testrec( PyMostPopular* self, PyObject* args, PyObject* kwdict );
PyObject* MostPopular_recommend( PyMostPopular* self, PyObject* args, PyObject* kwdict );


#endif // __PY_MOST_POPULAR_H__

