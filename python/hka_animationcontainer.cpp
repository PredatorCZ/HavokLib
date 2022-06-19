#include "hka_animationcontainer.hpp"
#include "hka_skeleton.hpp"

static PyMappingMethods hkaAniContSkeletonListPyMappingMethods[] = {
    (lenfunc)hkaAniContSkeletonListPy::Len,
    (binaryfunc)hkaAniContSkeletonListPy::Subscript,
    0,
};

static PyTypeObject hkaAniContSkeletonListPyType = {
    PyVarObject_HEAD_INIT(NULL, 0)                    /* init macro */
    "hkaAniContSkeletonList",                         /* tp_name */
    sizeof(hkaAniContSkeletonListPy),                 /* tp_basicsize */
    0,                                                /* tp_itemsize */
    0,                                                /* tp_dealloc */
    0,                                                /* tp_print */
    0,                                                /* tp_getattr */
    0,                                                /* tp_setattr */
    0,                                                /* tp_compare */
    0,                                                /* tp_repr */
    0,                                                /* tp_as_number */
    0,                                                /* tp_as_sequence */
    hkaAniContSkeletonListPyMappingMethods,           /* tp_as_mapping */
    0,                                                /* tp_hash */
    0,                                                /* tp_call */
    0,                                                /* tp_str */
    0,                                                /* tp_getattro */
    0,                                                /* tp_setattro */
    0,                                                /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT,      /* tp_flags */
    "Havok Animation Container Skeleton Iterator",    /* tp_doc */
    0,                                                /* tp_traverse */
    0,                                                /* tp_clear */
    0,                                                /* tp_richcompare */
    0,                                                /* tp_weaklistoffset */
    (getiterfunc)hkaAniContSkeletonListPy::Iter,      /* tp_iter */
    (iternextfunc)hkaAniContSkeletonListPy::IterNext, /* tp_iternext */
    0,                                                /* tp_methods */
    0,                                                /* tp_members */
    0,                                                /* tp_getset */
    0,                                                /* tp_base */
    0,                                                /* tp_dict */
    0,                                                /* tp_descr_get */
    0,                                                /* tp_descr_set */
    0,                                                /* tp_dictoffset */
    0,                                                /* tp_init */
    0,                                                /* tp_alloc */
    0,                                                /* tp_new */
};

size_t hkaAniContSkeletonListPy::Len(hkaAniContSkeletonListPy *self) {
  return self->hkClass->GetNumSkeletons();
}

PyObject *hkaAniContSkeletonListPy::Subscript(hkaAniContSkeletonListPy *self,
                                              PyObject *index) {
  const auto id = PyInt_AsSsize_t(index);
  return self->SubscriptRaw(id);
}

PyObject *hkaAniContSkeletonListPy::SubscriptRaw(size_t index) {
  return hkaSkeletonPy::Alloc(hkClass->GetSkeleton(index));
}

PyObject *hkaAniContSkeletonListPy::Iter(hkaAniContSkeletonListPy *self) {
  self->iterPos = 0;
  return reinterpret_cast<PyObject *>(self);
}

PyObject *hkaAniContSkeletonListPy::IterNext(hkaAniContSkeletonListPy *self) {
  if (self->iterPos < self->Len(self)) {
    return self->SubscriptRaw(self->iterPos++);
  } else {
    PyErr_SetNone(PyExc_StopIteration);
    return nullptr;
  }
}

static PyMethodDef hkaAnimationContainerPyMethods[] = {
    {"skeletons", (PyCFunction)hkaAnimationContainerPy::Skeletons, METH_NOARGS,
     "Get skeleton objects."},
    {NULL},
};

static PyTypeObject hkaAnimationContainerPyType = {
    PyVarObject_HEAD_INIT(NULL, 0)               /* init macro */
    "hkaAnimationContainer",                     /* tp_name */
    sizeof(hkaAnimationContainerPy),             /* tp_basicsize */
    0,                                           /* tp_itemsize */
    0,                                           /* tp_dealloc */
    0,                                           /* tp_print */
    0,                                           /* tp_getattr */
    0,                                           /* tp_setattr */
    0,                                           /* tp_compare */
    0,                                           /* tp_repr */
    0,                                           /* tp_as_number */
    0,                                           /* tp_as_sequence */
    0,                                           /* tp_as_mapping */
    0,                                           /* tp_hash */
    0,                                           /* tp_call */
    0,                                           /* tp_str */
    0,                                           /* tp_getattro */
    0,                                           /* tp_setattro */
    0,                                           /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT, /* tp_flags */
    "Havok Animation Container",                 /* tp_doc */
    0,                                           /* tp_traverse */
    0,                                           /* tp_clear */
    0,                                           /* tp_richcompare */
    0,                                           /* tp_weaklistoffset */
    0,                                           /* tp_iter */
    0,                                           /* tp_iternext */
    hkaAnimationContainerPyMethods,              /* tp_methods */
    0,                                           /* tp_members */
    0,                                           /* tp_getset */
    0,                                           /* tp_base */
    0,                                           /* tp_dict */
    0,                                           /* tp_descr_get */
    0,                                           /* tp_descr_set */
    0,                                           /* tp_dictoffset */
    0,                                           /* tp_init */
    0,                                           /* tp_alloc */
    0,                                           /* tp_new */
};

PyObject *hkaAnimationContainerPy::Skeletons(hkaAnimationContainerPy *self) {
    hkaAniContSkeletonListPy *rtVal = reinterpret_cast<hkaAniContSkeletonListPy *>(
      PyType_GenericNew(&hkaAniContSkeletonListPyType, nullptr, nullptr));
    rtVal->hkClass = self->hkClass;

    return reinterpret_cast<PyObject *>(rtVal);
}


void hkaAnimationContainerPy::InitType(PyObject *module) {
  if (PyType_Ready(&hkaAnimationContainerPyType) < 0)
    return;
  Py_INCREF(&hkaAnimationContainerPyType);
  PyModule_AddObject(module, hkaAnimationContainerPyType.tp_name,
                     (PyObject *)&hkaAnimationContainerPyType);

  if (PyType_Ready(&hkaAniContSkeletonListPyType) < 0)
    return;
  Py_INCREF(&hkaAniContSkeletonListPyType);
  PyModule_AddObject(module, hkaAniContSkeletonListPyType.tp_name,
                     (PyObject *)&hkaAniContSkeletonListPyType);
}

PyObject *hkaAnimationContainerPy::Alloc(const IhkVirtualClass *cls) {
  auto cted = reinterpret_cast<hkaAnimationContainerPy *>(
      PyType_GenericAlloc(&hkaAnimationContainerPyType, 0));
  cted->hkClass = checked_deref_cast<const hkaAnimationContainer>(cls);

  return reinterpret_cast<PyObject *>(cted);
}
