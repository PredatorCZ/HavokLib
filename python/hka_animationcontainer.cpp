#include "hka_animationcontainer.hpp"
#include "hka_skeleton.hpp"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

static PyMappingMethods hkaAniContSkeletonListPyMappingMethods{
    (lenfunc)hkaAniContSkeletonListPy::Len,
    (binaryfunc)hkaAniContSkeletonListPy::Subscript,
    0,
};

PyTypeObject *hkaAniContSkeletonListPy::GetType() {
  static PyTypeObject hkaAniContSkeletonListPyType{
      .tp_name = "hkaAniContSkeletonList",
      .tp_basicsize = sizeof(hkaAniContSkeletonListPy),
      .tp_as_mapping = &hkaAniContSkeletonListPyMappingMethods,
      .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT,
      .tp_doc = "Havok Animation Container Skeleton Iterator",
      .tp_iter = (getiterfunc)hkaAniContSkeletonListPy::Iter,
      .tp_iternext = (iternextfunc)hkaAniContSkeletonListPy::IterNext,
  };

  return &hkaAniContSkeletonListPyType;
}

size_t hkaAniContSkeletonListPy::Len(hkaAniContSkeletonListPy *self) {
  return self->hkClass->GetNumSkeletons();
}

PyObject *hkaAniContSkeletonListPy::Subscript(hkaAniContSkeletonListPy *self,
                                              PyObject *index) {
  const auto id = PyLong_AsSize_t(index);
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

PyTypeObject *hkaAnimationContainerPy::GetType() {
  static PyTypeObject hkaAnimationContainerPyType{
      .tp_name = "hkaAnimationContainer",
      .tp_basicsize = sizeof(hkaAnimationContainerPy),
      .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_IS_ABSTRACT,
      .tp_doc = "Havok Animation Container",
      .tp_methods = hkaAnimationContainerPyMethods,
  };

  return &hkaAnimationContainerPyType;
}

PyObject *hkaAnimationContainerPy::Skeletons(hkaAnimationContainerPy *self,
                                             void *) {
  hkaAniContSkeletonListPy *rtVal =
      reinterpret_cast<hkaAniContSkeletonListPy *>(PyType_GenericNew(
          hkaAniContSkeletonListPy::GetType(), nullptr, nullptr));
  rtVal->hkClass = self->hkClass;

  return reinterpret_cast<PyObject *>(rtVal);
}

PyObject *hkaAnimationContainerPy::Alloc(const IhkVirtualClass *cls) {
  auto cted = reinterpret_cast<hkaAnimationContainerPy *>(
      PyType_GenericAlloc(GetType(), 0));
  cted->hkClass = checked_deref_cast<const hkaAnimationContainer>(cls);

  return reinterpret_cast<PyObject *>(cted);
}

void hkaAnimationContainerPy::InitType(PyObject *module) {
  PyAddTypes<hkaAnimationContainerPy, hkaAniContSkeletonListPy>(module);
}
