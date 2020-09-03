#ifndef __PY_CONJUGATE_GRADIENT_H__
#define __PY_CONJUGATE_GRADIENT_H__

#include <Python.h> // This header was set at the first place to avoid warnings

#include "AlgIFAlsConjugateGradient.h"
#include "DataReader.h"
#include "DataFrame.h"
#include "MAE.h"
#include "RMSE.h"

class PyIFAlsConjugateGradient
{
public:

   PyObject_HEAD
   DataReader* m_trainingReader;
   int m_nusers;
   int m_nitems;
   DataFrame* m_pTestData;
   AlgIFAlsConjugateGradient* m_recAlgorithm;
};

PyTypeObject* IFAlsConjugateGradientGetType();
PyObject* IFAlsConjugateGradientNew( PyTypeObject* type, PyObject* args, PyObject* kwdict );
PyObject* IFAlsConjugateGradientTrain( PyIFAlsConjugateGradient* self, PyObject* args, PyObject* kwdict );

#endif // __PY_CONJUGATE_GRADIENT_H__

